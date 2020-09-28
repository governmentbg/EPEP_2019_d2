#include "cac.h"

#include "rc51.c"

// ----- TXferElectric -------------------------------------------------------
struct TXferElectric
{
	TXferElectric(const char *tName, int tType);
	~TXferElectric() { delete[] data; }

	bool ReadData();
	void EmitData(TXMLFile &x);

	char name[SIZE_OF_PATH];
	int type;
	char *data;
	long size;
	mstr ext;
};

TXferElectric::TXferElectric(const char *tName, int tType)
	: type(tType), data(NULL), size(0)
{
	strcpy(name, tName);
}

bool TXferElectric::ReadData()
{
	data = TUserFile(Electrics[type].what).ReadAll(name, size);

	if (!data)
		return false;

	if (size < 2)
		return error("%s: файлът е прекалено кратък.", name);

	ext.cpy(has_word_ext(name) ? get_proper_word_ext(data, size) : file_ext(name));
	ext.upper();
	return true;
}

void TXferElectric::EmitData(TXMLFile &x)
{
	// 2017:181 LPR: new schema allows 4 characters
	//ext.cut(3);
	x.Emit("Type", str(ext));
	x.EmitBase64("Content", data, size);
}

// ----- TPrintSubpoena ------------------------------------------------------
enum
{
	LEGAL_AID_SPECIAL_REPRESENTATIVE	= 0x01,
	LEGAL_AID_RESERVE_DEFENDER		= 0x02,
	LEGAL_AID_ON_DUTY				= 0x04,
	LEGAL_AID_CONFLICTING_INTERESTS	= 0x08
};

class TPrintSubpoena : public TSubpoena
{
public:
	TPrintSubpoena(TWindow *parent, TSubpoena *subpoena, TRCDKeyContainer *tContainer, TGArray<TLinkWind> *tLinks,
		TGArray<TLegalSide> *tLaids, TSubpoena *tSentSubpoena, TGArray<TSideWind> *tSideWinds, const char *tCategories,
		mstr &tReceiver);
	virtual ~TPrintSubpoena();

	void RequestCC();
	void LawsuitCC();
	void OutRegCC();
	void JudgementCC(TSubpoena *subpoena, bool decision);
	void GetEvent(TInReg *inReg);
	void Connect(mstr &m, char connectType, const char *prefix);	// '\0' -> TryLower
	bool FutureFixed(TSession *session) const;
	void ExecAccounts(msql &w, const char *fields);

	TRCDKeyContainer *container;
	TRCDKey origin;

	TRCDKey requestKey;
	CDate requestDate;

	char eventKind;
	long eventNo;
	CDate eventDate;
	mstr eventText;
	char resolution;
	char judge[SIZE_OF_UCN];

	char inRegKind;
	char inRegReceiver[SIZE_OF_UCN];
	char originKind;
	char requestKind;
	char result;

	TGArray<TDecision> decisions;
	TGArray<TSideWind> &sideWinds;
	const char *categories;
	CDate decForceDate;
	CDate judForceDate;
	CDate lawDate;

	char college;
	long composition;
	char text[SIZE_OF_ORIGIN_TEXT];
	long subject;
	long lawQuick;
	long lawFlagsY;

	TGArray<TLinkWind> *links;	// 2008:176 multi-subpoena
	// 2009:213
	mstr additionalText;
	char appeal[SIZE_OF_APPEAL_TEXT];
	double summary;
	TRCDKeyContainer *transcript;
	char accomply;			// 2010:330
	TSubpoena *sentSubpoena;	// 2012:073
	TText *execText;			// 2012:082
	const char *unwanted;		// 2013:280 LPR: instead of side re-get
	bool statext;			// 2014:073 IRQ: +related
	char lawsuitType;			// 2014:085 FIX: +related

	// 2012:038
	TSideWind *man;
	TSideWind *woman;
	TSideWind *FindSpouse(bool woman);

	// 2013:337+
	long legalAidType;
	long legalAidFlags;
	TGArray<TLegalSide> *laids;
	mstr otherAuthorizeLawer;
	TIArray<TXferElectric> electrics;

	bool XferDuplicate();
	bool XferCollect(int electricType, const TRCDKeyContainer *container);
	sign XferCollectAll();
	void XferElectrics(TXMLFile &x);
	void XferLegalAid();

	mstr description;				// 2014:209 +related
	long recipient;				// 2015:083
	mstr &receiver;				// 2016:244 for e-portal
	CDate inRegDate;				// 2016:278
	long lawerCount;				// 2017:346 +related

	// 2016:089
	bool execSumsLoaded;
	TGArray<TExecListSum> execListSums;

	void LoadExecSums();
	void PrintExecSums(mstr &m, bool decoration);
};

TPrintSubpoena::TPrintSubpoena(TWindow *parent, TSubpoena *subpoena, TRCDKeyContainer *tContainer,
	TGArray<TLinkWind> *tLinks, TGArray<TLegalSide> *tLaids, TSubpoena *tSentSubpoena, TGArray<TSideWind> *tSideWinds,
	const char *tCategories, mstr &tReceiver)
	: container(tContainer), origin(tContainer->key), eventKind(tContainer->kind), eventNo(0), eventDate(tContainer->date),
		originKind('\0'), inRegKind('\0'), college('\0'), composition(0), links(tLinks), laids(tLaids), resolution('\0'),
		result('\0'), categories(tCategories), summary(0), requestKind('\0'), transcript(NULL),
		sentSubpoena(tSentSubpoena), sideWinds(*tSideWinds), statext(false), lawsuitType('\0'),
		execSumsLoaded(false), receiver(tReceiver), lawQuick(0), lawFlagsY(0)
{
	TWaitWindow wait(parent, WAIT);
	*judge = '\0';
	*inRegReceiver = '\0';
	*text = '\0';	// 2017:027 legal aid

	TLong *tEventNo = (TLong *) container->Find("F_EVENT_NO");
	TChar *tResolution = (TChar *) container->Find("F_RESOLUTION");

	if (tEventNo)
		eventNo = tEventNo->L();
	if (tResolution)
		resolution = tResolution->C();

	memset(&requestKey, '\0', sizeof requestKey);
	*this << subpoena;

	if (strchr(KIND_SESSIONS, eventKind))
	{
		TSession *session = TYPECHECK_PTR_CAST(TSession, container);

		LawsuitCC();

		composition = session->composition;
		result = session->result;
		eventText.cpy(session->text);
		strcpy(judge, session->judge);

		if (try2decide(session, &decisions, NULL, "F_KIND, F_TEXT, F_FROM_DATE, F_FORCE_DATE"))
			JudgementCC(subpoena, false);
		else
		{
			if (session->IndexType())
				eventKind = session->IndexKind();
			// 2013:235 FIX: was missing
			decForceDate = session->forceDate;
		}

		if (subpoenaKind == SUBPOENA_KIND_SETTSESS_COPY)
			eventKind = KIND_SETTLEMENT;
		else if (subpoenaKind == SUBPOENA_KIND_ORDSESS_COPY)
			eventKind = KIND_ORDER;

		if (!eventNo && strchr(KIND_ENDOCS, eventKind))
		{
			TDecision decision;

			decision << container;
			decision.kind = eventKind;

			if (decision.Try("F_EVENT_NO, F_FROM_DATE, F_KIND, F_TEXT"))
			{
				eventNo = decision.eventNo;
				eventDate = decision.fromDate;
				eventText.cpy(decision.text);
			}
		}
	}
	else if (strchr(KIND_ENDOCS, eventKind))
	{
		TDecision *decision = (TDecision *) container;

		LawsuitCC();

		eventDate = decision->fromDate;
		decForceDate = decision->forceDate;
		decisions.Add(decision);
		JudgementCC(subpoena, true);
	}
	else if (eventKind == KIND_REQUEST_RESOLUTION)
	{
		TRequestResolution *resolution = (TRequestResolution *) container;
		requestKey = origin;
		RequestCC();
		originKind = requestKind;
		eventText.cpy(resolution->text);
		if (atob(resolution->judge))
			strcpy(judge, resolution->judge);
	}
	else if (strchr(KIND_INREG_EVENTS, eventKind))
	{
		TInRegEvent *inRegEvent = (TInRegEvent *) container;
		TInReg inReg;

		inReg.key = origin;
		result = inRegEvent->result;

		if (inReg.Get())
		{
			origin = inReg.lawsuit;
			// 20180-6-26: check for non-zero origin.type
			if (origin.type && strchr(TYPE_LAWSUITS, origin.type))
				LawsuitCC();
			college = inReg.college;
			inRegKind = inReg.kind;
			inRegDate = inReg.date;
			strcpy(inRegReceiver, inReg.receiver);

			SubpoenaKind->Seek(subpoenaKind);
			if (SubpoenaKind->IndirectEvent())
				GetEvent(&inReg);
		}

		eventText.cpy(inRegEvent->text);
		if (atob(inRegEvent->judge))
			strcpy(judge, inRegEvent->judge);
	}

	// fallback
	if (!college)
	{
		if (lawsuitType)
			college = lawsuitType;
		else if (origin.type && strchr(TYPE_LAWSUITS, origin.type))
			college = lawsuitType = origin.type;
		else if (strchr(TYPE_LAWSUITS, container->key.type))
			college = lawsuitType = container->key.type;
	}

	SubpoenaKind->Seek(subpoenaKind);
	if (SubpoenaKind->proc == SK_PROC_LETTER_TO)
		OutRegCC();

	Add(
		new TText("F_ADDITIONAL_TEXT", &additionalText, SIZE_OF_LARGE_TEXT),
		new TString("F_APPEAL", appeal, sizeof appeal),
		new TCharAlias("F_ACCOMPLY", &accomply),
		NULL
	);

	accomply = ACCOMPLY_NONE;

	// 2013:337+
	Add(
		new TCheck("F_SPECIAL_REPRESENTATIVE", &legalAidFlags, LEGAL_AID_SPECIAL_REPRESENTATIVE),
		new TCheck("F_RESERVE_DEFENDER", &legalAidFlags, LEGAL_AID_RESERVE_DEFENDER),
		new TCheck("F_ON_DUTY", &legalAidFlags, LEGAL_AID_ON_DUTY),
		new TCheck("F_CONFLICTING_INTERESTS", &legalAidFlags, LEGAL_AID_CONFLICTING_INTERESTS),
		new TText("F_OTHER_AUTHORIZE_LAWYER", &otherAuthorizeLawer, SIZE_OF_LEGAL_AID_TEXT),
		new TText("F_DESCRIPTION", &description, SIZE_OF_LEGAL_AID_DESCRIPTION),
		new TLong("F_LAWER_COUNT", &lawerCount),
		NULL
	);

	lawerCount = LAWER_COUNT_MIN;
}

TPrintSubpoena::~TPrintSubpoena()
{
	delete transcript;
	if (decisions.Count() && decisions[0] == container)
		decisions.Detach(0);
}

void TPrintSubpoena::JudgementCC(TSubpoena *subpoena, bool decision)
{
	TJudgement judgement;

	judgement << subpoena;
	judgement.date = container->date;

	for(int i = 0; i < decisions.Count(); i++)
	{
		const TDecision *tDecision = decisions[i];

		judgement.kind = tDecision->kind;
		eventKind = tDecision->kind;

		if (any(eventText))
			eventText.cat("\v");

		if (decision && judgement.Try("F_TEXT, F_FORCE_DATE"))
		{
			eventText.cpy(judgement.text);
			judForceDate = judgement.forceDate;
		}
		else
		{
			eventText.cat(tDecision->text);
			eventNo = tDecision->eventNo;
			eventDate = tDecision->fromDate;
			decForceDate = tDecision->forceDate;
		}
	}
}

void TPrintSubpoena::RequestCC()
{
	TRequest request;
	request.key = requestKey;

	if (request.Get("F_KIND, F_COLLEGE, F_DATE, F_SUBJECT, F_TEXT, F_LAW_KIND"))
	{
		requestKind = request.kind;
		requestDate = request.date;
		subject = request.subject;
		lawsuitType = kind2type(request.lawKind);
	}
}

void TPrintSubpoena::LawsuitCC()
{
	TLawsuit lawsuit;
	lawsuit.key = origin;

	if (lawsuit.Get("F_DATE, F_KIND, F_COLLEGE, F_COMPOSITION, F_SOURCE_NO, F_SOURCE_YEAR, F_SOURCE_TYPE, F_SUBJECT, "
		"F_TEXT, F_JUDGE, F_QUICK, F_FLAGS_Y"))
	{
		lawDate = lawsuit.date;
		composition = lawsuit.composition;
		requestKey = lawsuit.source;
		originKind = lawsuit.kind;
		strcpy(judge, lawsuit.judge);
		if (requestKey.type == TYPE_REQUEST)
			RequestCC();
		// override any request data
		college = lawsuit.college;
		strcpy(text, lawsuit.text);
		subject = lawsuit.subject;
		lawsuitType = origin.type;
		lawQuick = lawsuit.quick;
		lawFlagsY = lawsuit.flagsY;
	}
}

void TPrintSubpoena::OutRegCC()
{
	TSubpoena tSubpoena;
	TBaseWind baseWind(tSubpoena.Name, NULL);
	msql m;

	baseWind << this;
	baseWind.ExactCriteria(m, "F_OUTREG_NO, F_OUTREG_YEAR, F_SUBPOENA_KIND");
	m.AddChar("F_EVENT_STATUS", eventStatus);		// 2013:143 LPR: sync w/ HasRegards()
	m.AddLong("F_SUBPOENA_NO", regard);

	TQuery q(str(m));
	if (q.Read())
	{
		tSubpoena << q;
		outRegNo = tSubpoena.outRegNo;
		outRegYear = tSubpoena.outRegYear;
		statext = strchr(SUBPOENA_KIND_EXEC_LETTERS, subpoenaKind) &&
			tSubpoena.subpoenaKind == SUBPOENA_KIND_EXEC_COUNTRY;
	}
}

void TPrintSubpoena::GetEvent(TInReg *inReg)
{
	eventKind = inReg->decisionKind;
	eventDate = inReg->decisionDate;

	if (eventKind && !eventDate.Empty())
	{
		TIndexContainer *const container = inReg->AppealedAct(true, true);

		if (container)
		{
			TVirtPtr VP(container);
			eventNo = container->eventNo;
			eventKind = container->kind;
			eventDate = container->date;
		}
	}
}

void TPrintSubpoena::Connect(mstr &m, char connectType, const char *prefix)
{
	TSender sender;
	TConnect connect;

	if (connectType ? connect.TryBy(key, connectType) : connect.TryLower(key))
	{
		bool floor = connectType == CONNECT_TYPE_FLOOR_LAWSUIT;
		bool penalty = connectType == CONNECT_TYPE_PENALTY_ENACTMENT || connectType == CONNECT_TYPE_ELECTRIC_APFISH;

		m.sep(prefix);
		ConnectType->Seek(connect.connectType);
		m.printf("%s № %s/%dг.", floor ? ConnectType->alias : penalty ? ConnectType->name : connect.connectKind ?
			ConnectKind->Seek(connect.connectKind) : "", connect.connectNo, connect.connectYear);

		floor = penalty ? penalty : floor;

		strcpy(sender.ucn, connect.ucn);
		sender.ucnType = connect.ucnType;
		if (sender.Try())
			m.printf(floor ? " %s" : " по описа на %s", sender.name);
	}
	// 2013:245
	else if (connectType == CONNECT_TYPE_PRECOURT_LAWSUIT)
		m.cat(ELLIPSIS_STD);
}

bool TPrintSubpoena::FutureFixed(TSession *session) const
{
	msql m;

	session->key = container->key;
	session->FloatCriteria(m, "F_DATE, F_TIME, F_HALL");

	if (container->date == Today)
	{
		m.Add("(F_DATE");
		m.printf("> %ld", Today.Value());
		m.NextOr();
		m.AddDate("(F_DATE", Today);
		m.Add("F_TIME");
		m.printf("> %ld", LNow().Value());
		m.cat("))");
	}
	else
	{
		m.Add("F_DATE");
		m.printf("> %ld", container->date.Value());
	}

	m.AddChar("F_RESULT", RESULT_FIXED);
	m.AddOrder("F_DATE");

	TQuery q(str(m));
	if (q.Read())
	{
		*session << q;
		return true;
	}

	return false;
}

static unsigned char rc5_key[16] = { 0x86, 0xFC, 0x4D, 0x3B, 0x79, 0x9A, 0x7E, 0xB5, 0xE5, 0x6D, 0xB0, 0xAA, 0x95, 0x44,
	0x72, 0xE0 };
static symmetric_key sym_key;

void printsub_init()
{
#if TESTVER  // TESTVER
	// even values mistakenly trigger CG on decode
	int result = rc5_setup(rc5_key, sizeof rc5_key, 19, &sym_key);

	if (result)
		fatal("rc5_setup = %d", result);
#else  // TESTVER
	rc5_setup(rc5_key, sizeof rc5_key, 18, &sym_key);
#endif  // TESTVER
}

static void rc5_encode(const char *ucn, unsigned long output[2])
{
	char tUCN[SIZE_OF_UCN];
	unsigned long input[2];

	strcpy(tUCN, ucn);
	tUCN[LENGTH_OF_UCN - 1] = '\0';
	input[1] = atoi(tUCN);
	srand(input[1]);
	input[0] = rand();
	rc5_ecb_encrypt((const unsigned char *) input, (unsigned char *) output, &sym_key);
#if TESTVER
	unsigned long check[2];

	rc5_ecb_decrypt((const unsigned char *) output, (unsigned char *) check, &sym_key);
	if (check[0] != input[0] || check[1] != input[1])
		fatal("rc5_encode: input %lx, %lx != test %lx, %lx", input[0], input[1], check[0], check[1]);
#endif  // TESTVER
}

void printsub_emit_creator(TXMLFile &x, const char *tag)
{
	unsigned long creatorId[2];
	mstr m;

	m.clear();
	rc5_encode(Default->ucn, creatorId);
	m.printf("<%s EmployeeID=\"%08lx%08lx\">", tag, creatorId[0], creatorId[1]);
	x.PutRaw(str(m), true);
	m.clear();
	generic_full_user_name(m, NULL);
	x.Put(str(m));
	m.clear();
	m.printf("</%s>\n", tag);
	x.PutRaw(str(m), false);
}

bool TPrintSubpoena::XferDuplicate()
{
	TSubpoena tSubpoena;
	msql m(tSubpoena.Name);
	TIntegerArray checked;

	m.Add(this, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_EVENT_STATUS");
	m.AddChar("F_SUBPOENA_KIND", subpoenaKind);
	m.AddIsNull("F_OUTREG_NO", false);

	TQuery q(str(m));
	while (q.Read())
	{
		tSubpoena << q;

		if (checked.Find(tSubpoena.requestId) != INT_MAX)
			continue;

		for (int i = 0; i < laids->Count(); i++)
		{
			TLegalSide *legalSide = (*laids)[i];

			if (legalSide->ucnType == tSubpoena.ucnType && !strcmp(legalSide->ucn, tSubpoena.ucn))
			{
				const char *status = legal_aid_status(&tSubpoena);

				if (status)
				{
					m.cpy("За лицето ");
					tSubpoena.PrintNames(m);
					error("%s вече има издадено %s, %s, изходящ номер %ld/%ldг.", str(m),
						SubpoenaKind->Seek(subpoenaKind), status, tSubpoena.outRegNo, tSubpoena.outRegYear);
					return true;
				}
				// no reason to check for other laids
				break;
			}
		}

		checked.Add(tSubpoena.requestId);
	}

	return false;
}

bool TPrintSubpoena::XferCollect(int electricType, const TRCDKeyContainer *container)
{
	char aid_name[SIZE_OF_PATH];
	bool aid_exist;

	// 2016:194 URQ/IRQ: always send PDF if any
	// 2016:221 FIX: set and use electricType
	electric_name(container, aid_name, electricType);
	aid_exist = exist(aid_name);

	if (!aid_exist)
	{
		electricType = TYPE_TEXT;
		electric_name(container, aid_name, electricType);
		aid_exist = exist(aid_name);
	}

	if (aid_exist)
		electrics.Add(new TXferElectric(aid_name, electricType));

	return aid_exist;
}

sign TPrintSubpoena::XferCollectAll()
{
	// PASS 1: session
	if (!XferCollect(TYPE_PORTREG, container))
	{
		return warn("Данните за Искането са некомплектни - липсва подходящ присъединен документ. "
			"Ще бъдат генерирани отделни искания за всяко лице, без електронен обмен с програма "
			"\"Служебни защити\".\n\nЖелаете ли да продължите?") ? false : -1;
	}

	// PASS 2: surroundments
	TSurroundment surroundment;
	msql m(surroundment.Name, "F_NO, F_YEAR, F_TYPE, F_KIND");
	TSurround surround;

	if (strchr(KIND_SESSIONS, container->kind))
		surroundment.SessionCriteria(m, TYPECHECK_PTR_CAST(TSession, container));
	else
	{
		const TDecision *decision = TYPECHECK_PTR_CAST(TDecision, container);
		surroundment.SessionCriteria(m, decision->key, decision->sessionKind, decision->date);
	}

	TQuery q(str(m));
	while (q.Read())
	{
		const char *format = NULL;

		surroundment << q;
		surround.key = surroundment.key;

		if (!surround.Try("F_KIND, F_DATE"))
			format = "Грешка при четене на %s %ld/%ldг";
		else if (!XferCollect(TYPE_IMAGE, &surround))
			format = "Данните за Искането са некомплектни - към %s %ld/%ldг. липсва подходящ присъединен документ";

		if (format)
		{
			m.clear();
			m.printf(format, Kind->Seek(surroundment.kind), surroundment.key.no, surroundment.key.year);
			return warn("%s. Ще бъдат генерирани отделни искания за всяко лице, без електронен обмен с програма "
				"\"Служебни защити\".\n\nЖелаете ли да продължите?", str(m)) ? false : -1;
		}
	}

	return true;
}

void TPrintSubpoena::XferElectrics(TXMLFile &x)
{
	for (int index = 0; index < electrics.Count(); index++)
	{
		mstr m;

		m.printf("Document id=\"%d\"", index + 1);
		x.OpenTag(str(m));
		electrics[index]->EmitData(x);
		x.CloseTag("Document");
	}
}

void TPrintSubpoena::XferLegalAid()
{
	char name[SIZE_OF_PATH];
	char tRequestId[SIZE_OF_LEGAL_AID_ID];
	TXMLFile x;
	mstr m;

	// 2017:171 moved here from print_subpoena()
	for (int i = 0; i < electrics.Count(); i++)
		if (!electrics[i]->ReadData())
			return;

	requestId = GenerateRequestId(DataBase);
	subpoena_legal_name(name, tRequestId, &container->key, LEGAL_AID_CREATED, requestId);

	if (x.OpenLegal(m, name, container))
	{
		x.OpenTag("Requests");
		{
			x.OpenTag("Request");
			{
				x.Emit("RequestDate", Today);
				m.cpy(SubpoenaKind->Seek(subpoenaKind));
				if (!strncmpi(str(m), "Искане по ", 10))
				{
					m.delead(10);
					*ncstr(m) = toupper(*str(m));
				}
				x.Emit("Reason", str(m));
				x.Emit("ReserveDefender", (legalAidFlags & LEGAL_AID_RESERVE_DEFENDER) != 0);
				x.Emit("OnDuty", (legalAidFlags & LEGAL_AID_ON_DUTY) != 0);
				if (lawerCount > 0)
					x.Emit("LawyerCount", lawerCount);
				if (legalAidType)
					x.Emit("TypeLegalAid", legalAidType);
				if (any(description))
					x.Emit("Description", str(description));

				printsub_emit_creator(x, "CreatedBy");
				x.Emit("RequestID", tRequestId);

				x.OpenTag("CaseData");
				{
					TSession session;

					x.PrintF("CaseNumber", "%ld/%ld", origin.no, origin.year);
					x.Emit("CaseDate", lawDate);
					if (FutureFixed(&session))
					{
						x.PrintF("SittingDate", "%04ld-%02d-%02dT%0lT", session.date.year,
							session.date.month, session.date.day, &session.time);
					}
					x.Emit("CaseIdentifier", Kind->Xfer(originKind));
					x.Emit("Subject", Subject->Seek(lawsuitType, subject));

					if (session.hall)
					{
						Hall->Seek(session.hall);
						if (any(Hall->address))
							x.Emit("CasePlace", Hall->address);
						else
						{
							m.clear();
							generic_court_address(m, NULL);
							x.Emit("CasePlace", str(m));
						}
					}
				}
				x.CloseTag("CaseData");

				x.OpenTag("InstanceData");
				{
					x.Emit("InstanceIdentifier", COURT_CODE);

					m.clear();
					if (PRINT_COLLEGE)
						print_college(m, college);
					if (PRINT_COMPOSITIONS)
					{
						m.sep("/");
						print_composition(m, composition);
					}
					if (any(m))
						x.Emit("AdditionalData", str(m));

					x.OpenTag("Employee");
					{
						m.clear();
						print_user(m, judge);
						x.Emit("Name", str(m));
						x.Emit("Position", "Съдия-докладчик");
					}
					x.CloseTag("Employee");
				}
				x.CloseTag("InstanceData");

				x.Emit("SpecialRepresentative", (legalAidFlags & LEGAL_AID_SPECIAL_REPRESENTATIVE) != 0);

				x.OpenTag("RequestAdditionalData");
				{
					x.Emit("ConflictingInterests", (legalAidFlags & LEGAL_AID_CONFLICTING_INTERESTS) != 0);
					x.Emit("OtherAuthorizeLawyer", str(otherAuthorizeLawer));
					// 2014:058 optional
					if (eventNo)
						x.PrintF("Appendix", "%s %ld от %Dг.", Kind->Seek(eventKind), eventNo, &eventDate);
				}
				x.CloseTag("RequestAdditionalData");

				x.OpenTag("Documents");
				{
					XferElectrics(x);
				}					
				x.CloseTag("Documents");

				x.OpenTag("Litigants");
				{
					for (int i = 0; i < laids->Count(); i++)
					{
						TLegalSide *legalSide = (*laids)[i];

						x.OpenTag("Litigant");
						{
							m.clear();
							legalSide->PrintNames(m);
							x.Emit("Name", str(m));
							if (legalSide->ucnType == UCN_CITIZEN_UCN)
								x.Emit("EGN", legalSide->ucn);
							m.clear();
							legalSide->address->Print(m, ADDRESS_DISTINCT | ADDRESS_REMARK);
							x.Emit("Address", str(m));
							x.Emit("Involvement", Involvement->Xfer(legalSide->involvement));
							x.Emit("IsSpecifiedLawyer", any(legalSide->specifiedLawer));
							if (any(legalSide->specifiedLawer))
								x.Emit("SpecifiedLawyer", legalSide->specifiedLawer);
							x.PrintF("LitigantID", "%c%s", legalSide->ucnType, legalSide->ucn);
						}
						x.CloseTag("Litigant");
					}
				}
				x.CloseTag("Litigants");
			}
			x.CloseTag("Request");
		}
		x.CloseTag("Requests");
		x.Close();

		m.clear();
		m.printf("legalaid: %s", basename(name));
		log(LOG_TRANSFER, 0, &container->key, str(m));
	}
}

void TPrintSubpoena::ExecAccounts(msql &w, const char *fields)
{
	w.Begin("T_EXEC_LIST E JOIN T_EXEC_LIST_SUM T ON T.F_EXEC_LIST_NO = E.F_EXEC_LIST_NO "
		"AND T.F_EXEC_LIST_YEAR = E.F_EXEC_LIST_YEAR AND T.F_TYPE = E.F_TYPE "
		"AND T.F_EXEC_LIST_KIND = E.F_EXEC_LIST_KIND JOIN T_ACCOUNT A ON A.F_UCN = T.F_SENDER", fields);

	w.AddLong("E.F_NO", key.no);
	w.AddLong("E.F_YEAR", key.year);
	w.AddChar("E.F_TYPE", key.type);
	w.AddLong("E.F_EXEC_LIST_NO", sentSubpoena->execListNo);
	w.AddDate("E.F_EXEC_LIST_DATE", sentSubpoena->subpoenaDate);
	w.AddLong("E.F_EXEC_LIST_KIND", sentSubpoena->execListKind);
	w.AddChar("A.F_UCN_TYPE", UCN_EXEC_LIST);
}

void TPrintSubpoena::LoadExecSums()
{
	if (!execSumsLoaded)
	{
		TExecListSum sum;
		msql w;

		sum.Criteria(sentSubpoena, w);
		sum.LoadArray(execListSums, str(w));
		execSumsLoaded = true;
	}
}

void TPrintSubpoena::PrintExecSums(mstr &m, bool decoration)
{
	LoadExecSums();

	for (int i = 0; i < execListSums.Count(); i++)
	{
		TExecListSum *sum = execListSums[i];

		m.sep(decoration ? "\v" : ", ");
		// 2014:314 support for value-less
		if (strchr(SUM_TYPE_VALUES, sum->sumType))
		{
			strcpy(Account->ucn, sum->sender);
			Account->ucnType = UCN_EXEC_LIST;

			if (decoration && atob(sum->sender) && Account->Get())
			{
				m.printf("по сметка на %s %s, %s, BIC %s сумата от ", Account->name, Account->bankName,
					Account->bankAccount, Account->bic);
			}

			mstr w;

			sum_in_words(sum->sum, w, sum->currency, true);
			Currency->Seek(sum->currency);
			m.printf("%.2lf %s (словом: %s)", sum->sum, Currency->numeric, str(w));

			if (decoration)
			{
				m.cat(", представляваща ");
				print_sum_type_lower(m, sum);
			}
		}
		else
		{
			if (decoration)
				m.cat("Непарично задължение ");

			print_sum_type_lower(m, sum);

			if (decoration && any(sum->sumText))
				m.printf(" %s", sum->sumText);
		}
	}
}

TSideWind *TPrintSubpoena::FindSpouse(bool woman)
{
	for (int i = 0; i < sideWinds.Count(); i++)
	{
		if (categories[i] != SIDEWIND_THIRD)	// 2013:138 IRQ/TRQ: FIX
		{
			TSideWind *sideWind = sideWinds[i];

			if (sideWind->ucnType == UCN_CITIZEN_UCN && (sideWind->ucn[8] - '0') % 2 == woman)
				return sideWind;
		}
	}

	return NULL;
}

void print_side(mstr &m, TSideWind *sideWind, long flags)
{
	bool found = false;
	TFirm firm;
	TCitizen citizen;

	if (sideWind && strchr(UCN_CITIZENS, sideWind->ucnType))
	{
		citizen << sideWind;
		found = citizen.Try();
	}

	if (sideWind && strchr(UCN_FIRMS, sideWind->ucnType))
	{
		firm << sideWind;
		found = firm.Try();
	}

	if (found)
	{
		if (flags & PRINT_SUB_ANY_NAME)
		{
			if (strchr(UCN_CITIZENS, sideWind->ucnType))
			{
				m.printf("%s%s", citizen.Rank(), citizen.name);

				if (flags & PRINT_SUB_SHORT_NAME)
				{
					if (strlen(citizen.reName) > 2)
						strcpy(citizen.reName + 1, ".");

					if (any(citizen.reName))
						m.printf(" %s", citizen.reName);
				}
				else
					m.printf(" %s", citizen.reName);

				m.printf(" %s", citizen.family);

				if ((flags & PRINT_SUB_ARBITARY_NAME) && any(citizen.reFamily))
					m.printf(" %s", citizen.reFamily);
			}
			else
				m.printf(" %s", firm.name);
		}

		if (flags & PRINT_SUB_UCN_BULSTAT)
		{
			m.cach(' ');

			if (is_real_ucn(sideWind))
				m.cat(sideWind->ucn);
			else if (strchr(UCN_FIRMS, sideWind->ucnType) && any(firm.bulstat))
				m.cat(firm.bulstat);
			else
				m.cat(ELLIPSIS_STD);
		}

		if (flags & PRINT_SUB_ADDRESS)
		{
			TAddress address;

			address.ucnType = sideWind->ucnType;
			strcpy(address.ucn, sideWind->ucn);

			if (address.Try())
				address.Print(m);
		}

		if (flags & PRINT_SUB_INVOLVEMENT)
		{
			if (strchr(UCN_FIRMS, sideWind->ucnType))
			{
				msql w;
				bool links = false;
				TLinkWind linkWind;

				linkWind.SetOrdinary(sideWind);
				linkWind.FloatCriteriaByOrdinary(w);
				TQuery q(str(w));

				while (q.Read())
				{
					linkWind << q;
					TSideWind tSideWind;

					tSideWind.ucnType = linkWind.proxyUcnType;
					strcpy(tSideWind.ucn, linkWind.proxyUcn);
					tSideWind.involvement = linkWind.proxyInvolvement;

					if (links)
						m.cat(", ");

					print_side(m, &tSideWind, PRINT_SUB_ARBITARY_NAME);
					m.printf(" - %s", Involvement->Seek(tSideWind.involvement));
					links = true;
				}

				if (!links)
				{
					m.cach(' ');
					m.cat(ELLIPSIS_STD);
				}
			}
			else
				m.printf(" - %s", Involvement->Seek(sideWind->involvement));
		}
	}
}

static void print_side_sep(mstr &m, TSideWind *sideWind, int flags)
{
	m.sep(", ");
	print_side(m, sideWind, flags);
}

static void print_sides(mstr &m, TGArray<TSideWind> &sideWinds, char category, const char *categories, long flags)
{
	for (int i = 0; i < sideWinds.Count(); i++)
		if ((categories ? categories[i] : sideWinds[i]->involvement) == category)
			print_side_sep(m, sideWinds[i], flags);
}

void print_laids(mstr &m, TGroupArray *array, bool involvement, const char *separator)
{
	for (int i = 0; i < array->Count(); i++)
	{
		TBaseWind *baseWind = TYPECHECK_PTR_CAST(TBaseWind, (*array)[i]);
		TExecLink *execLink = dynamic_cast<TExecLink *> (baseWind);
		TTryAddress *tryAddress = dynamic_cast<TTryAddress *> (baseWind);

		if (i)
			m.cat(separator);

		if (execLink)
			print_side_links(m, execLink);
		else
			baseWind->PrintNames(m);

		m.cat(", ");

		if (strchr(UCN_CITIZENS, baseWind->ucnType))
			m.printf("ЕГН %s", baseWind->ucnType == UCN_CITIZEN_UCN ? baseWind->ucn : ELLIPSIS_STD);
		else
		{
			TFirm firm;

			firm << baseWind;
			firm.Get();
			m.printf("ЕИК %s", any(firm.bulstat) ? firm.bulstat : ELLIPSIS_STD);
		}

		if (tryAddress->address)
		{
			m.cach(' ');
			tryAddress->address->Print(m, ADDRESS_DISTINCT | ADDRESS_REMARK);
		}

		if (involvement && baseWind->involvement)
			m.printf(", %s", Involvement->Seek(baseWind->involvement));
	}
}

static void print_schedule_session(mstr &m, TPrintSubpoena *printsub, const char *format, bool hall)
{
	TSession tSession;

	if (printsub->result == RESULT_FIXED)
		tSession << printsub->container;
	else if (printsub->key.type != TYPE_REQUEST)	// 2017:027 legal aid
		printsub->FutureFixed(&tSession);

	if (!tSession.date.Empty() && !tSession.time.Empty())
	{
		m.printf(format, &tSession.date, &tSession.time);

		if (hall && tSession.hall)
			m.printf(" %s", Hall->Seek(tSession.hall));
	}
}

static void print_barcode(mstr &m, TPrintSubpoena *printsub, bool asterisk)
{
	if (!printsub->requestId)
		printsub->requestId = printsub->GenerateRequestId(printsub->DataBase);

	barcode_generate(m, printsub->requestId, asterisk);
}

static void print_judge(mstr &m, TPrintSubpoena *printsub, bool name_family)
{
	TCitizen citizen;

	citizen.ucnType = UCN_CITIZEN_UCN;
	strcpy(citizen.ucn, printsub->judge);

	if (!atob(printsub->judge) || !citizen.Try())
		return;

	#if COURT_TYPE == COURT_MILITARY
		m.cat(citizen.Rank());
	#endif  // MILITARY

	if (name_family)
		m.printf("%s %s", citizen.name, citizen.family);
	else
	{
	#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		if (strchr(SUBPOENA_KIND_CONVICT_BULLETS, printsub->subpoenaKind))
			m.printf("%s %s %s", citizen.name, citizen.reName, citizen.family);
		else
	#endif  // AREAL || MILITARY || REGIONAL
		{	// 2017:115 LRQ: "N. FAMILY" for exec lists
			m.printf(strchr(SUBPOENA_KIND_EXEC_LISTS, printsub->subpoenaKind) ? "%c. %s" : "%c.%s",
				*citizen.name, citizen.family);
		}
	}
}

#define printsub OFFICE_GROUP_CAST(TPrintSubpoena)

static void printsub_college(mstr &m, TGroup *group) { if (PRINT_COLLEGE) print_college(m, printsub->college); }
static void printsub_composition(mstr &m, TGroup *group) { if (PRINT_COMPOSITIONS) print_composition(m, printsub->composition); }
static void printsub_origin_no(mstr &m, TGroup *group) { itom(flag_key_no(&printsub->origin), m); }
static void printsub_origin_year(mstr &m, TGroup *group) { itom(printsub->origin.year, m); }
static void printsub_origin_type(mstr &m, TGroup *group) { m.cpy(Type->Seek(printsub->origin.type)); }

#define container (printsub->container)

static void printsub_container_date(mstr &m, TGroup *group) { container_date(m, container); }
static void printsub_container_time(mstr &m, TGroup *group) { container_time(m, container); }

static void printsub_connects(mstr &m, TGroup *group)
{
	TGArray<TConnect> connects;
	TSender sender;

	load_connects(container, &connects);
	for (int i = 0; i < connects.Count(); i++)
	{
		TConnect *connect = connects[i];

		if (strchr(CONNECT_TYPE_PRINTSUBS, connect->connectType))
		{
			sender.ucnType = connect->ucnType;
			strcpy(sender.ucn, connect->ucn);
			if (!sender.Get())
				strcpy(sender.name, sender.ucn);

			m.sep(", ");
			ConnectType->Seek(connect->connectType);
			m.printf("%s %s/%ldг. %s", ConnectType->alias, connect->connectNo, connect->connectYear, sender.name);
		}
	}
}

#define session TYPECHECK_PTR_CAST(TSession, container)

static void printsub_multi_date_time(mstr &m, TGroup *group)
{
	if (session->end.Empty() || session->end == session->date)
		m.printf("%D в %T часа", &session->date, &session->time);
	else
	{
		m.cat("\v");

		for (CDate date = session->date; date <= session->end; date += 1)
			m.printf("%D в %T часа\v", &date, &session->time);
	}
}

static void printsub_session_hall(mstr &m, TGroup *group)
{
	if (session->hall)
		m.cpy(Hall->Seek(session->hall));
}

static void printsub_hall_address(mstr &m, TGroup *group)
{
	if (session->hall)
	{
		Hall->Seek(session->hall);
		m.cpy(Hall->address);
	}
}

static void printsub_schedule_session(mstr &m, TGroup *group)
{
	print_schedule_session(m, printsub, "%Dг. от %T часа", true);
}

static void printsub_opt_schedule_session(mstr &m, TGroup *group)
{
	print_schedule_session(m, printsub, "насрочено за %Dг. от %Tч", false);
}

#undef session

static void printsub_event_no(mstr &m, TGroup *group)
{
	if (PRINT_JUDGEMENTS)
		itom(printsub->eventNo, m);
}

static void printsub_event_date(mstr &m, TGroup *group) { printsub->eventDate.Print(m); }

static void printsub_suitors(mstr &m, TGroup *group)
{
#if COURT_TYPE == COURT_AREAL
	if (printsub->origin.type == TYPE_FIRM_LAW && strchr(SUBPOENA_KIND_OLD_GPKS, printsub->subpoenaKind))
	{
		TLawsuit lawsuit;
		TDamn damn;

		lawsuit.key = printsub->origin;

		if (find_damn(&lawsuit, &damn, true))
			m.printf("%s %s", damn.name, CompanyType->Seek(damn.companyType));
	}
	else
#endif  // AREAL
	{
		bool fullName = CourtType == COURT_REGIONAL && printsub->subpoenaKind == SUBPOENA_KIND_MARRIAGE_CERT;
		print_sides(m, printsub->sideWinds, SIDEWIND_LEFT, printsub->categories, fullName ? PRINT_SUB_FULL_NAME : PRINT_SUB_SHORT_NAME);
	}
}

static void printsub_suitor_ucn(mstr &m, TGroup *group) { print_sides(m, printsub->sideWinds, SIDEWIND_LEFT, printsub->categories, PRINT_SUB_UCN_BULSTAT); }

static void printsub_defendants(mstr &m, TGroup *group)
{
	bool fullName = CourtType == COURT_REGIONAL && printsub->subpoenaKind == SUBPOENA_KIND_MARRIAGE_CERT;

	print_sides(m, printsub->sideWinds, SIDEWIND_RIGHT, printsub->categories, fullName ? PRINT_SUB_FULL_NAME : PRINT_SUB_SHORT_NAME);
}

static void printsub_defendant_ucn(mstr &m, TGroup *group) { print_sides(m, printsub->sideWinds, SIDEWIND_RIGHT, printsub->categories, PRINT_SUB_UCN_BULSTAT); }
static void printsub_request_no(mstr &m, TGroup *group) { itom(printsub->requestKey.no, m); }
static void printsub_request_year(mstr &m, TGroup *group) { itom(printsub->requestKey.year, m); }
static void printsub_request_date(mstr &m, TGroup *group) { printsub->requestDate.Print(m); }

static void printsub_event_kind(mstr &m, TGroup *group)
{
	// 2008:086 LPR/LRQ: check if empty
	if (printsub->eventKind)
	{
		// 2014:317 IRQ/LPR: settlement for un-indexed exec list session
		if (strchr(KIND_SESSIONS, printsub->eventKind) && !((TSession *) container)->IndexType())
			m.cpy(Kind->Seek(KIND_SETTLEMENT));
		else
			m.cpy(Kind->Seek(printsub->eventKind));
	}
}

static void printsub_inreg_kind(mstr &m, TGroup *group) { m.cpy(Kind->Seek(printsub->inRegKind)); }

static void printsub_origin_kind(mstr &m, TGroup *group)
{
	if (printsub->originKind)
		m.cpy(Kind->Seek(printsub->originKind));
	else
		printsub_origin_type(m, group);
}

static void printsub_instance_III(mstr &m, TGroup *group)
{
#if COURT_TYPE == COURT_AREAL
	// 2008:085 LRQ: specific slowdown receivers
	if (printsub->subpoenaKind == SUBPOENA_KIND_INREGAPP_COPY && printsub->inRegKind == KIND_INREG_SLOWDOWN)
	{
		if (strchr(KIND_2ND_CITIZEN_LAWS, printsub->originKind) || strchr(KIND_2ND_TRADE_LAWS, printsub->originKind))
			m.cpy(InstanceSuper);
		else if (KIND_ADMIN_LAWS, printsub->originKind)
			m.cpy(InstanceSpeal);
	}

	if (!any(m))
	// fallback
#endif  // AREAL
	generic_instance_III(m, group);
}

static void printsub_request_kind(mstr &m, TGroup *group)
{
	if (printsub->requestKind)
		m.cpy(Kind->Seek(printsub->requestKind));
	else
		m.cpy(Type->Seek(TYPE_REQUEST));
}

static void printsub_opposite_sides(mstr &m, TGroup *group)
{
	m.cpy(printsub->unwanted);

	char category = container->Category(printsub->involvement);

	if (category != SIDEWIND_THIRD)
		full_side_list(m, &printsub->sideWinds, printsub->categories, category == SIDEWIND_LEFT ? SIDEWIND_RIGHT : SIDEWIND_LEFT);
}

static void printsub_quick(mstr &m, TGroup *group)
{
	if (strchr(TYPE_LAWSUITS, container->key.type))
	{
		if (printsub->lawQuick & QUICK_QUICK)
			m.cat(printsub->lawQuick & QUICK_IMMEDIATE ? "Незабавно производство" : "Бързо производство");

#if COURT_TYPE == COURT_REGIONAL
		if (printsub->requestKind == KIND_EXECUTIVE_REQUEST)
		{
			m.sep(", ");
			m.cat("Заповедно производство");
		}
#endif  // REGIONAL
	}
}

#undef container

static void printsub_notifieds(mstr &m, TGroup *group)
{
	printsub->PrintNotifieds(m, printsub->links, "\v");
	// 2016:293 high priority
	printsub->receiver.cpy(m);
}

static void printsub_subpoena_date(mstr &m, TGroup *group) { printsub->subpoenaDate.Print(m); }
static void printsub_lower_lawsuit(mstr &m, TGroup *group) { printsub->Connect(m, '\0', ""); }
static void printsub_floor_lawsuit(mstr &m, TGroup *group) { printsub->Connect(m, CONNECT_TYPE_FLOOR_LAWSUIT, ""); }
static void printsub_precourt_lawsuit(mstr &m, TGroup *group) { printsub->Connect(m, CONNECT_TYPE_PRECOURT_LAWSUIT, ""); }

static void printsub_lower_decision(mstr &m, TGroup *group)
{
	TConnect connect;

	if (connect.TryLower(printsub->key, "F_DECISION_KIND") && connect.decisionKind)
		m.cat(Kind->Seek(connect.decisionKind));
}

static void printsub_lawsuit_text(mstr &m, TGroup *group) { m.cpy(printsub->text); }

static void printsub_subject(mstr &m, TGroup *group)
{
	if (printsub->subject)	// 2017:027 legal aid
		m.cpy(Subject->Seek(printsub->lawsuitType, printsub->subject));
}

static void printsub_renew_lawsuits(mstr &m, TGroup *group)
{
	printsub->Connect(m, CONNECT_TYPE_LOWER_INSTANCE, "");
	printsub->Connect(m, CONNECT_TYPE_RECALL_LAWSUIT, " и ");
}

static void printsub_general_kind(mstr &m, TGroup *group)
{
	Kind->Seek(kind2GeneralKind(printsub->requestKind));
	m.cpy(Kind->name);
}

static void printsub_event_status(mstr &m, TGroup *group)
{
	if (printsub->resolution)
		m.cpy(Resolution->Seek(printsub->resolution));
}

static void printsub_session_result(mstr &m, TGroup *group)
{
	m.cpy(Result->Seek(printsub->result));
}

static void printsub_event_text(mstr &m, TGroup *group) { m.cpy(printsub->eventText); }

static void printsub_reminder(mstr &m, TGroup *group)
{
	// 2015:0178 LPR: char[] -> msql, F_UCN = %s -> quoted
	TSubpoena subpoena;
	msql w(subpoena.Name, "MIN(F_SUBPOENA_DATE) AS F_SUBPOENA_DATE");

	w.Add(printsub, "F_NO, F_YEAR, F_TYPE, F_KIND, F_UCN, F_UCN_TYPE, F_SUBPOENA_KIND");
	TQuery q(str(w));

	if (q.Read())
	{
		subpoena << q;
		if (!subpoena.subpoenaDate.Empty())
			m.printf("НАПОМНИТЕЛНО! I-во СРГ - %Dг.", &subpoena.subpoenaDate);
	}
}

static void printsub_receiver(mstr &m, TGroup *group)
{
	subpoena_receiver(m, group);
	// 2016:293 low priority
	if (!any(printsub->receiver))
		printsub->receiver.cat(m);
}

static void printsub_receiver_ucn(mstr &m, TGroup *group)
{
	if (is_real_ucn(printsub))
		m.cat(printsub->ucn);
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN
	else
		m.cat(ELLIPSIS_STD);
#endif  // APPEAL || ADMIN
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	else if (!strchr(SUBPOENA_KIND_CONVICT_BULLETS, printsub->subpoenaKind))
		m.cat(ELLIPSIS_STD);
#endif  // AREAL || MILITARY || REGIONAL
}

static void printsub_receiver_birthday(mstr &m, TGroup *group)
{
	if (printsub->ucnType == UCN_CITIZEN_UCN)
		CDate(printsub->ucn).Print(m);
	else if (strchr(SUBPOENA_KIND_LETTER_ADDRESSES, printsub->subpoenaKind))
		m.cpy(ELLIPSIS_STD);
}

static void printsub_judge(mstr &m, TGroup *group) { print_judge(m, printsub, false); }
static void printsub_force_date(mstr &m, TGroup *group) { printsub->decForceDate.Print(m); }

static void printsub_force_date_text(mstr &m, TGroup *group)
{
	if (!printsub->decForceDate.Empty())
		m.printf(", влязло в законна сила на %Dг.,", &printsub->decForceDate);
}

static void printsub_legacy_leaver(mstr &m, TGroup *group)
{
	print_sides(m, printsub->sideWinds, SIDEWIND_RIGHT, printsub->categories, PRINT_SUB_SHORT_NAME);
}

static void printsub_legacy_leaver_ucn(mstr &m, TGroup *group)
{
	print_sides(m, printsub->sideWinds, SIDEWIND_RIGHT, printsub->categories, PRINT_SUB_UCN_BULSTAT);
}

static void printsub_legacy_leaver_uclp(mstr &m, TGroup *group)
{
	print_sides(m, printsub->sideWinds, SIDEWIND_RIGHT, printsub->categories, PRINT_SUB_ADDRESS);
}

static void printsub_lreceiver(mstr &m, TGroup *group)
{
	if (printsub->lReceiverUcnType && atof(printsub->lReceiverUcn))
		print_name(m, printsub->lReceiverUcn, printsub->lReceiverUcnType);
}

static void printsub_last_known_address(mstr &m, TGroup *group)
{
	TAddress address;
	address << group;

	if (address.Try())	// 2013:245 LPR: s -> m
		address.Print(m, ADDRESS_DISTINCT);
}

static void printsub_regard_subpoena_kind(mstr &m, TGroup *group)
{
	// 2016:089; 2016:130 IRQ/FIX: for COUTNRY exec lists only
	if (printsub->subpoenaKind == SUBPOENA_KIND_EXECLET_NIATDEP &&
		printsub->sentSubpoena->execListKind == EXEC_LIST_COUNTRY)
	{
		msql w;
		int recipient = 0;

		printsub->ExecAccounts(w, "DISTINCT A.F_FLAGS");
		w.AddOrder("A.F_ORDER");

		TQuery q(str(w));
		while (q.Read())
		{
			*Account << q;
			recipient |= Account->flags & ACCOUNT_JUDICIAL ? ACCOUNT_JUDICIAL : FLAG_ACTIVE;
		}

		m.cat("Изпълнителен лист в полза на ");
		print_recipient(m, recipient);

		if (recipient == 0 || recipient == ACCOUNT_JUDICIAL)
			m.printf(" по сметка на %s", AppNameCourt);
	}
	else	// standard
		m.cat(SubpoenaKind->Seek(printsub->sentSubpoena->subpoenaKind));
}

static void printsub_regard_subpoena_date(mstr &m, TGroup *group)
{
	printsub->sentSubpoena->subpoenaDate.Print(m);
}

static void printsub_regard_address(mstr &m, TGroup *group)
{
	printsub->sentSubpoena->PrintAddress(m, ADDRESS_DISTINCT | ADDRESS_REMARK);
}

static void printsub_court_area(mstr &m, TGroup *)
{
	UCLP->Seek(COURT_UCLP);
	m.cpy(Area->Seek(UCLP->area));
}

static void printsub_judge_post(mstr &m, TGroup *group)
{
	if (atob(printsub->judge))
	{
		TUser user;

		strcpy(user.ucn, printsub->judge);
		if (user.Try())
			m.cpy(Post->Seek(user.post));
	}
}

static void printsub_receiver_address(mstr &m, TGroup *group)
{
	printsub->PrintAddress(m, ADDRESS_DISTINCT | ADDRESS_REMARK);
}

static void printsub_receiver_country(mstr &m, TGroup *group)
{
	TCitizen citizen;

	citizen.ucnType = printsub->ucnType;
	strcpy(citizen.ucn, printsub->ucn);

	if (citizen.Try("F_COUNTRY"))
		m.cpy(Country->Seek(citizen.country));
}

static void printsub_personal_force_date(mstr &m, TGroup *group) { printsub->judForceDate.Print(m); }
static void printsub_additional_text(mstr &m, TGroup *group) { m.cpy(printsub->additionalText); }
static void printsub_appeal(mstr &m, TGroup *group) { m.cpy(printsub->appeal); }

static void printsub_punishment_enactment(mstr &m, TGroup *group)
{
	printsub->Connect(m, CONNECT_TYPE_PENALTY_ENACTMENT, "");
	printsub->Connect(m, CONNECT_TYPE_ELECTRIC_APFISH, "\v");
}

static void printsub_tax(mstr &m, TGroup *group)
{
	TExpense expense;
	msql w(expense.Name, "F_SUM_1, F_SUM_2");

	w.Add(printsub, "F_NO, F_YEAR, F_TYPE");
	w.AddChar("F_SUM_TYPE", SUM_TYPE_TAX);
	TQuery q(str(w));

	while (q.Read())
	{
		expense << q;
		printsub->summary += expense.sum1 + expense.sum2;
	}

	if (printsub->summary)
		m.printf("%12.2lf лв. (", printsub->summary);
	else
	{
		m.cach(' ');
		m.cat(ELLIPSIS_STD);
	}
}

static void printsub_tax_text(mstr &m, TGroup *group)
{
	if (printsub->summary)
	{
		sum_in_words(printsub->summary, m);
		m.cach(')');
	}
}

static void printsub_subpoena_outreg_no(mstr &m, TGroup *group)
{
	if (printsub->regard)
		itom(printsub->outRegNo, m);
}

static void printsub_outreg_date(mstr &m, TGroup *group)
{
	if (printsub->regard && printsub->outRegNo)
	{
		TOutReg outReg;
		msql w;

		outReg.lawsuit = printsub->key;
		outReg.FloatCriteria(w, "F_DATE");
		w.AddLong("F_NO", printsub->outRegNo);

		TQuery q(str(w));
		if (q.Read())
			outReg.date.Print(m);
	}
	else
		m.cat(ELLIPSIS_STD);
}

static void printsub_lawsuit_date(mstr &m, TGroup *group) { printsub->lawDate.Print(m); }

static void printsub_transcript_desc(mstr &m, TGroup *group)
{
	if (printsub->transcript)
	{
		m.cpy(strchr(KIND_ANOTHER_SURROUNDS, printsub->transcript->kind)
			? ((TBaseString *) printsub->transcript->Find("F_TEXT"))->S()
			: Kind->Seek(printsub->transcript->kind));
	}
}

static void printsub_transcript_no(mstr &m, TGroup *group)
{
	if (printsub->transcript)
		itom(printsub->transcript->key.no, m);
}

static void printsub_transcript_date(mstr &m, TGroup *group)
{
	if (printsub->transcript)
		printsub->transcript->date.Print(m);
}

static void printsub_transcript_senders(mstr &m, TGroup *group)
{
	if (printsub->transcript)
	{
		TGArray<TSideWind> sideWinds;
		load_sidebasicv(printsub->transcript, sideWinds);

		if (printsub->transcript->key.type == TYPE_REQUEST)
			print_sides(m, sideWinds, SIDEWIND_LEFT, printsub->categories, PRINT_SUB_SHORT_NAME);
		else	// surround
		{
			for (int i = 0; i < sideWinds.Count(); i++)
				if (sideWinds[i]->Appealer())
					print_side_sep(m, sideWinds[i], PRINT_SUB_SHORT_NAME);
		}
	}
}

static void printsub_deliverer(mstr &m, TGroup *group)
{
	if (atob(printsub->delivererUcn) && WRITINGS_ACTIVE != 2)	// 2018-03-19: support 2 
		print_user(m, printsub->delivererUcn);
	else
		m.cpy(ELLIPSIS(28));
}

static void printsub_accomply(mstr &m, TGroup *group) { m.cpy(Accomply->Seek(printsub->accomply)); }

static void printsub_spouse_ucn(mstr &m, TSideWind *sideWind)
{
	m.cat(sideWind ? sideWind->ucn : ELLIPSIS(20));
}

static void printsub_spouse_birthday(mstr &m, TSideWind *sideWind)
{
	if (sideWind)
		m.printf("%0D", &CDate(sideWind->ucn));
}

static void printsub_spouse_name(mstr &m, TSideWind *sideWind)
{
	if (sideWind)
		sideWind->PrintNames(m);
}

static void printsub_spouse_address(mstr &m, TSideWind *sideWind, int address)
{
	if (sideWind)
		print_exact_address(m, sideWind, address, false);
}

static void printsub_man_ucn(mstr &m, TGroup *group)
{
	printsub->man = printsub->FindSpouse(false);
	printsub_spouse_ucn(m, printsub->man);
}

static void printsub_man_birthday(mstr &m, TGroup *group) { printsub_spouse_birthday(m, printsub->man); }
static void printsub_man_name(mstr &m, TGroup *group) { printsub_spouse_name(m, printsub->man); }
static void printsub_man_address_1(mstr &m, TGroup *group) { printsub_spouse_address(m, printsub->man, 1); }
static void printsub_man_address_2(mstr &m, TGroup *group) { printsub_spouse_address(m, printsub->man, 2); }

static void printsub_woman_ucn(mstr &m, TGroup *group)
{
	printsub->woman = printsub->FindSpouse(true);
	printsub_spouse_ucn(m, printsub->woman);
}

static void printsub_woman_birthday(mstr &m, TGroup *group) { printsub_spouse_birthday(m, printsub->woman); }
static void printsub_woman_name(mstr &m, TGroup *group) { printsub_spouse_name(m, printsub->woman); }
static void printsub_woman_address_1(mstr &m, TGroup *group) { printsub_spouse_address(m, printsub->woman, 1); }
static void printsub_woman_address_2(mstr &m, TGroup *group) { printsub_spouse_address(m, printsub->woman, 2); }

static void printsub_exec_text(mstr &m, TGroup *group)
{
	if (printsub->execText)
		m.cat(printsub->execText->S());
}

#if COURT_TYPE == COURT_APPEAL
static void printsub_presense_required(mstr &m, TGroup *group)
{
	static constant presense[2] =
	{
		".\v\6\6\6\6\6\6\6\6\6\6Съгласно чл.329, ал.2 НПК, присъствието на подсъдимото лице е задължително",
		".\v\6\6\6\6\6\6\6\6\6\6Присъствието на лицето не е задължително. Ако същото изрично заяви, че не желае "
		"да се яви в съдебно заседание или довеждането му е невъзможно по здравословни причини, моля да бъдем "
		"уведомени по надлежния ред"
	};

	m.cat(presense[strchr(KIND_PUNISHMENT_LAWS, printsub->originKind) &&
		(strchr(KIND_KS_LAWSUITS, printsub->originKind) || strchr(KIND_RENEW_LAWS, printsub->originKind))]);
}
#endif  // APPEAL

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
static void printsub_presense_required(mstr &m, TGroup *group)
{
	static constant presense[2] =
	{
		", тъй като присъствието му е задължително",
		".\v\6\6\6\6\6\6\6\6\6\6Присъствието на лицето не е задължително. Ако същото изрично заяви, че не желае "
		"да се яви в съдебно заседание или довеждането му е невъзможно по здравословни причини, моля да бъдем "
		"уведомени по надлежния ред"
	};

	m.cat(presense[strchr(KIND_2ND_LAWSUITS, printsub->originKind) != NULL]);
}
#endif  // ADMIN || AREAL || MILITARY

#if COURT_TYPE == COURT_REGIONAL
static void printsub_presense_required(mstr &m, TGroup *)
{
	m.cat(", тъй като присъствието му е задължително");
}
#endif  // REGIONAL

static void printsub_budget_account(mstr &m, TGroup *) { print_court_account(m, ACCOUNT_TYPE_BUDGET, "F_BANK_ACCOUNT"); }
static void printsub_budget_bic(mstr &m, TGroup *) { print_court_account(m, ACCOUNT_TYPE_BUDGET, "F_BIC"); }
static void printsub_budget_bank_name(mstr &m, TGroup *) { print_court_account(m, ACCOUNT_TYPE_BUDGET, "F_BANK_NAME"); }
static void printsub_deposits_account(mstr &m, TGroup *) { print_court_account(m, ACCOUNT_TYPE_DEPOSITS, "F_BANK_ACCOUNT"); }
static void printsub_deposits_bic(mstr &m, TGroup *) { print_court_account(m, ACCOUNT_TYPE_DEPOSITS, "F_BIC"); }
static void printsub_deposits_bank_name(mstr &m, TGroup *) { print_court_account(m, ACCOUNT_TYPE_DEPOSITS, "F_BANK_NAME"); }
static void printsub_legal_aid_sides(mstr &m, TGroup *group) { print_laids(m, printsub->laids, false, "\v"); }
static void printsub_recipients(mstr &m, TGroup *group) { print_laids(m, printsub->links, true, "\v"); }

static void printsub_specified_lawer(mstr &m, TGroup *group)
{
	for (int i = 0; i < printsub->laids->Count(); i++)
	{
		TLegalSide *legalSide = (*printsub->laids)[i];

		if (any(legalSide->specifiedLawer))
		{
			m.sep("\v");
			legalSide->PrintNames(m);
			m.printf(" - Адвокат по чл.25, ал.5 %s", legalSide->specifiedLawer);
		}
	}
}

static void printsub_exec_list_no(mstr &m, TGroup *group)
{
	if (printsub->execListNo)
		print_exec_list_no(m, printsub->execListNo, printsub->execListKind);
	else
		print_exec_list_no(m, printsub->sentSubpoena->execListNo, printsub->sentSubpoena->execListKind);
}

static void printsub_debtors(mstr &m, TGroup *group)
{
	TGArray<TLegalSide> laids;

	if (printsub->sentSubpoena->execListKind == EXEC_LIST_COUNTRY)
	{
		msql m(printsub->sentSubpoena->Name);

		m.Add(printsub->sentSubpoena, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_SUBPOENA_NO");
		TQuery q(str(m));

		while (q.Read())
		{
			TLegalSide *legalSide = new TLegalSide;

			*legalSide << q;
			legalSide->address = new TAddress;
			*legalSide->address << q;
			laids.Add(legalSide);
		}
	}
	else
	{
		TExecListPerson person;
		msql m;
		TLegalSide laid;

		person.Criteria(printsub->sentSubpoena, m, "F_UCN, F_UCN_TYPE");
		laid.LoadArray(laids, str(m));
	}

	print_laids(m, &laids, false, "\v");
}

static void printsub_exec_sums(mstr &m, TGroup *group) { printsub->PrintExecSums(m, false); }
static void printsub_recipient(mstr &m, TGroup *group) { print_recipient(m, printsub->recipient); }

static void printsub_receiver_involvement(mstr &m, TGroup *group)
{
	TLinkWind *linkWind = printsub;	// single/self link

	if (linkWind->Proxied() && linkWind->proxyKind == SIDE_MARK_AS_WITH_INVOLVEMENT)
		; // 2014:272 involvements already shown in F_RECEIVER
	else
		basewind_involvement(m, group);
}

static void printsub_barcode_font_size(mstr &m, TGroup *) { itom(BARCODE_FONT_SIZE, m); }
static void printsub_barcode_font(mstr &m, TGroup *) { m.cat(BARCODE_FONT); }

static void printsub_barcode(mstr &m, TGroup *group)
{
	if (PRINT_BARCODES)
		print_barcode(m, printsub, BARCODE_ASTERISK);
}

static void printsub_bardigit_font_size(mstr &m, TGroup *) { itom(BARDIGIT_FONT_SIZE, m); }
static void printsub_bardigit_font(mstr &m, TGroup *) { m.cat(BARDIGIT_FONT); }

static void printsub_bardigit(mstr &m, TGroup *group)
{
	if (PRINT_BARCODES == TRUE)
		print_barcode(m, printsub, BARCODE_ASTERISK == 2);
}

static void printsub_returned(mstr &m, TGroup *group)
{
	printsub->container->Find("F_RETURNED")->Print(m);
}

static void printsub_csjid(mstr &m, TGroup *group)
{
	if (strchr(UCN_CITIZENS, printsub->lReceiverUcnType))
		print_known_person_csjid(m, printsub->lReceiverUcn, printsub->lReceiverUcnType);
}

static void printsub_judge_initials(mstr &m, TGroup *group)
{
	if (atob(printsub->judge))
		print_initials(m, printsub->judge, UCN_CITIZEN_UCN, true);
}

static void printsub_inreg_receiver(mstr &m, TGroup *group)
{
	if (atob(printsub->inRegReceiver))
		print_name(m, printsub->inRegReceiver, UCN_INSTANCE_HIGHER);
	else
		printsub_instance_III(m, group);
}

static void printsub_bank_accounts(mstr &m, TGroup *group)
{
	msql w;

	printsub->ExecAccounts(w, "DISTINCT A.*");
	w.AddOrder("A.F_ORDER");

	TQuery q(str(w));
	while (q.Read())
	{
		*Account << q;
		m.sep("\v");
		m.printf("%s, банкова сметка %s, BIC %s, банка %s", Account->name, Account->bankAccount, Account->bic,
			Account->bankName);
	}
}

static void printsub_execution(mstr &m, TGroup *group)
{
	m.cat(printsub->accomply == ACCOMPLY_IMMED ? "незабавно изпълнение" : "изпълнение");
}

static void printsub_exec_sums_accounts(mstr &m, TGroup *group) { printsub->PrintExecSums(m, true); }

static void printsub_exec_sums_total(mstr &m, TGroup *group)
{
	TFloatExecListTotalArray execSumTotal;

	printsub->LoadExecSums();	// 2016:103 IRQ: FIX: was missing

	for (int i = 0; i < printsub->execListSums.Count(); i++)
	{
		TExecListSum *sum = printsub->execListSums[i];

		if (strchr(SUM_TYPE_VALUES, sum->sumType))
			execSumTotal.AddSum(sum);
	}

	execSumTotal.Print(m, ", ");
}

#if COURT_TYPE == COURT_AREAL
static void printsub_meeting_type(mstr &m, TGroup *group)
{
	switch (printsub->accomply)
	{
		case ACCOMPLY_MEET_1ST : m.cat("първо"); break;
		case ACCOMPLY_MEET_LAST : m.cat("заключително");
	}
}

static void printsub_target_debtor(mstr &m, TGroup *group)
{
	printsub->PrintNames(m);
}

static void printsub_target_bulstat(mstr &m, TGroup *group)
{
	if (strchr(UCN_FIRMS, printsub->ucnType))
	{
		TFirm firm;

		firm.ucnType = printsub->ucnType;
		strcpy(firm.ucn, printsub->ucn);

		if (firm.Try("F_BULSTAT") && any(firm.bulstat))
			m.printf("ЕИК %s", firm.bulstat);
	}
}

static void printsub_meeting_agenda(mstr &m, TGroup *group)
{
	switch (printsub->accomply)
	{
		case ACCOMPLY_MEET_1ST : m.cat(
			"1.\tИзслушване на доклада на временния синдик по чл.668 т.2. от ТЗ.\v"
			"2.\tИзбор на постоянен синдик и предложение до съда за неговото назначаване, предлагане на съда "
				"размера на възнаграждението на синдика.\v"
			"3.\tИзбор на комитет на дружеството.");
			break;
		case ACCOMPLY_MEET_LAST : m.cat(
			"1.\tИзслушване на доклада на синдика за извършените разпределения на суми, "
				"събрани при осребряването, и за останалите неплатени вземания.\v"
			"2.\tИзслушване на отчета на синдика за неговата дейност."); break;
		default : m.cat(
			"1.\tОпределяне реда и начина за осребряване на имуществото на длъжника.\v"
			"2.\tОпределяне метода и условията на оценка на имуществото.\v"
			"3.\tИзбор на оценители и определяне на възнаграждението им.\v"
			"4.\tИзбор на постоянен синдик на дружеството.");
	}
}
#endif  // AREAL

static void printsub_first_set_no_year(mstr &m, TGroup *group)
{
	if (printsub->kind == KIND_1ST_SESSION)
	{
		TSession session;
		msql w(session.Name, "F_DATE, F_EVENT_NO, F_FLAGS_X");

		w.Add(printsub, "F_NO, F_YEAR, F_TYPE");

		w.Add("(F_DATE");
		w.printf("< %ld", printsub->date.Value());
		w.NextOr();
		w.AddDate("(F_DATE", printsub->date);
		w.AddChar("NOT F_KIND", printsub->kind);
		w.cat("))");

		w.AddChar("(F_KIND", printsub->kind);
		w.NextOr();
		w.AddFlags("F_FLAGS_X", SESSION_X_FIRSET, SESSION_X_FIRSET, SESSION_X_ALL);
		w.cat(")");

		w.AddOrder("F_DATE DESC");
		TQuery q(str(w));

		while (q.Read())
		{
			session << q;

			if (session.flagsX & SESSION_X_FIRSET)
			{
				if (PRINT_JUDGEMENTS)
					itom(session.eventNo, m);
				else
					m.cat("..............");

				m.printf("/ %ld", session.date.year);
				return;
			}

			if (session.date != printsub->date)
				break;
		}
	}

	m.cat("............../ ..............");
}

static void printsub_inreg_date(mstr &m, TGroup *group) { printsub->inRegDate.Print(m); }
static void printsub_inreg_sender(mstr &m, TGroup *group) { print_inreg_appealers(m, (TInReg *) printsub->container); }
static void printsub_college_initials(mstr &m, TGroup *group) { print_college_initials(m, printsub->college); }
static void printsub_judge_name(mstr &m, TGroup *group) { print_judge(m, printsub, true); }

static void printsub_user_name(mstr &m, TGroup *group)
{
	if (strchr(SUBPOENA_KIND_EXEC_LISTS, printsub->subpoenaKind))
		print_user(m, Default->ucn);
	else
		generic_user_name(m, group);
}

static void printsub_valid_date(mstr &m, TGroup *group) { printsub->validUntil.Print(m); }

static void printsub_regard_addr_skip_uclp(mstr &m, TGroup *group)
{
	printsub->sentSubpoena->PrintAddress(m, ADDRESS_DISTINCT | ADDRESS_REMARK | ADDRESS_SKIP_UCLP);
}

static void printsub_regard_addr_uclp_only(mstr &m, TGroup *group)
{
	m.cat(UCLP->Seek(printsub->sentSubpoena->uclp));
}

#undef printsub

static const offield printsub_fields[] =
{
	{ "F_JUDGE_TYPE",			generic_judge_type },
	{ "F_JUDGE_TYPE_COURT",		generic_judge_type_court },
	{ "F_COURT_PHONE",		generic_court_phone },
	{ "F_COURT_EMAIL",		generic_court_email },
	{ "F_COURT_AREA",			printsub_court_area },
	{ "F_LAWSUIT_NO",			printsub_origin_no },
	{ "F_LAWSUIT_YEAR",		printsub_origin_year },
	{ "F_LAWSUIT_TYPE",		printsub_origin_type },
	{ "F_LAWSUIT_KIND",		printsub_origin_kind },
	{ "F_LAWSUIT_TEXT",		printsub_lawsuit_text, },
	{ "F_USER_NAME",			printsub_user_name },
	{ "F_USER_INITIALS",		generic_user_initials },
	{ "F_FULL_USER_NAME",		generic_full_user_name },
	{ "F_RECEIVER",			printsub_receiver },
	{ "F_RECEIVER_UCN",		printsub_receiver_ucn },
	{ "F_RECEIVER_BIRTHDAY",	printsub_receiver_birthday },
	{ "F_RECEIVER_ADDRESS",		printsub_receiver_address },
	{ "F_RECEIVER_COUNTRY",		printsub_receiver_country },
	{ "F_SESSION_DATE",		printsub_container_date },
	{ "F_SESSION_TIME",		printsub_container_time },
	{ "F_SESSION_RESULT", 		printsub_session_result },
	{ "F_MULTI_DATE_TIME",		printsub_multi_date_time },	// 2013:259
	{ "F_HALL",				printsub_session_hall },
	{ "F_HALL_ADDRESS",		printsub_hall_address },
	{ "F_EVENT_NO",			printsub_event_no },
	{ "F_EVENT_DATE",			printsub_event_date },
	{ "F_EVENT_KIND",			printsub_event_kind },
	{ "F_EVENT_TEXT",			printsub_event_text },
	{ "F_EVENT_STATUS",		printsub_event_status },
	{ "F_SUITOR",			printsub_suitors },
	{ "F_SUITOR_UCN",			printsub_suitor_ucn },
	{ "F_SUITORS",			printsub_suitors },
	{ "F_DEFENDANT",			printsub_defendants },
	{ "F_DEFENDANT_UCN",		printsub_defendant_ucn },
	{ "F_DEFENDANTS",			printsub_defendants },
	{ "F_REQUEST_NO",			printsub_request_no },
	{ "F_REQUEST_DATE",		printsub_request_date },
	{ "F_REQUEST_YEAR",		printsub_request_year },
	{ "F_REQUEST_KIND",		printsub_request_kind },
	{ "F_INREG_KIND",			printsub_inreg_kind },
	{ "F_INREG_NO",			basewind_key_no },
	{ "F_INREG_YEAR",			basewind_key_year },
	{ "F_JUDGE", 			printsub_judge },
	{ "F_JUDGE_POST", 		printsub_judge_post },
	{ "F_LRECEIVER",			printsub_lreceiver },
	{ "F_LAST_KNOWN_ADDRESS",	printsub_last_known_address },
	{ "F_LOWER_LAWSUIT", 		printsub_lower_lawsuit },
	{ "F_RENEW_LAWSUITS", 		printsub_renew_lawsuits },
	{ "F_LOWER_DECISION_KIND", 	printsub_lower_decision },
	{ "F_LEGACY_LEAVER", 		printsub_legacy_leaver },
	{ "F_LEGACY_LEAVER_UCN", 	printsub_legacy_leaver_ucn },
	{ "F_LEGACY_LEAVER_UCLP", 	printsub_legacy_leaver_uclp },
	{ "F_REGARD_SUBPOENA_KIND", 	printsub_regard_subpoena_kind },
	{ "F_REGARD_SUBPOENA_DATE", 	printsub_regard_subpoena_date },
	{ "F_INVOLVEMENT",		basewind_involvement },
	{ "F_COLLEGE",			printsub_college },
	{ "F_COMPOSITION",		printsub_composition },
	{ "F_INSTANCE_III",		printsub_instance_III },
	{ "F_OPPOSITE_SIDES",		printsub_opposite_sides },
	{ "F_NOTIFIEDS",			printsub_notifieds },
	{ "F_SUBPOENA_DATE",		printsub_subpoena_date },
	{ "F_CONNECTS",			printsub_connects },
	{ "F_SUBJECT", 			printsub_subject },
	{ "F_FLOOR_LAWSUIT",		printsub_floor_lawsuit },
	{ "F_PRECOURT_LAWSUIT",		printsub_precourt_lawsuit },
	{ "F_GENERAL_REQUEST_KIND", 	printsub_general_kind },
	{ "F_REMINDER", 			printsub_reminder },
	{ "F_FORCE_DATE",  		printsub_force_date },
	{ "F_FORCE_DATE_TEXT",  	printsub_force_date_text },	// 2014:317 for exec lists
	{ "F_SIDE_NO",			subpoena_side_no },
	{ "F_ADDITIONAL_TEXT",		printsub_additional_text },
	{ "F_APPEAL",			printsub_appeal },
	{ "F_PERSONAL_FORCE_DATE",	printsub_personal_force_date },
	{ "F_QUICK",			printsub_quick },
	{ "F_SCHEDULE_SESSION",		printsub_schedule_session },
	{ "F_OPT_SCHEDULE_SESSION",	printsub_opt_schedule_session },
	{ "F_PUNISHMENT_ENACTMENT",	printsub_punishment_enactment },
	{ "F_TAX",				printsub_tax },
	{ "F_TAX_TEXT",			printsub_tax_text },
	{ "F_SUBPOENA_OUTREG_NO",	printsub_subpoena_outreg_no },
	{ "F_OUTREG_DATE",		printsub_outreg_date },
	{ "F_LAWSUIT_DATE",		printsub_lawsuit_date },
	{ "F_TRANSCRIPT_DESC",		printsub_transcript_desc },
	{ "F_TRANSCRIPT_NO",		printsub_transcript_no },
	{ "F_TRANSCRIPT_DATE",		printsub_transcript_date },
	{ "F_TRANSCRIPT_SENDERS",	printsub_transcript_senders },
	{ "F_DELIVERER",			printsub_deliverer },
	{ "F_ACCOMPLY",			printsub_accomply },
	// 2012:038
	{ "F_COURT_AREA",			generic_court_area },
	{ "F_COURT_REGION",		generic_court_region },
	{ "F_MAN_UCN",			printsub_man_ucn },
	{ "F_MAN_BIRTHDAY",		printsub_man_birthday },
	{ "F_MAN_NAME",			printsub_man_name },
	{ "F_MAN_ADDRESS_1",		printsub_man_address_1 },
	{ "F_MAN_ADDRESS_2",		printsub_man_address_2 },
	{ "F_WOMAN_UCN",			printsub_woman_ucn },
	{ "F_WOMAN_BIRTHDAY",		printsub_woman_birthday },
	{ "F_WOMAN_NAME",			printsub_woman_name },
	{ "F_WOMAN_ADDRESS_1",		printsub_woman_address_1 },
	{ "F_WOMAN_ADDRESS_2",		printsub_woman_address_2 },
	{ "F_REGARD_ADDRESS",		printsub_regard_address },
	{ "F_EXEC_TEXT",			printsub_exec_text },
	{ "F_PRESENSE_REQUIRED",	printsub_presense_required },
	// 2013:234
	{ "F_BUDGET_ACCOUNT",		printsub_budget_account },
	{ "F_BUDGET_BIC",			printsub_budget_bic },
	{ "F_BUDGET_BANK_NAME",		printsub_budget_bank_name },
	{ "F_DEPOSITS_ACCOUNT",		printsub_deposits_account },
	{ "F_DEPOSITS_BIC",		printsub_deposits_bic },
	{ "F_DEPOSITS_BANK_NAME",	printsub_deposits_bank_name },
	// 2013:339
	{ "F_LEGAL_AID_SIDES",		printsub_legal_aid_sides },
	{ "F_SPECIFIED_LAWYER",		printsub_specified_lawer },
	{ "F_OTHER_AUTHORIZE_LAWYER",	NULL },
	{ "F_RECIPIENTS",			printsub_recipients },
	{ "F_EXEC_LIST_NO",		printsub_exec_list_no },
	{ "F_DEBTORS",			printsub_debtors },
	{ "F_EXEC_SUMS",			printsub_exec_sums },
	{ "F_RECIPIENT",			printsub_recipient },
	{ "F_RECEIVER_INVOLVEMENT",	printsub_receiver_involvement },	// 2014:272
	{ "F_BARCODE_FONT_SIZE",	printsub_barcode_font_size },
	{ "F_BARCODE_FONT",		printsub_barcode_font },
	{ "F_BARCODE",			printsub_barcode },
	{ "F_BARDIGIT_FONT_SIZE",	printsub_bardigit_font_size },
	{ "F_BARDIGIT_FONT",		printsub_bardigit_font },
	{ "F_BARDIGIT",			printsub_bardigit },
	{ "F_RETURNED",			printsub_returned },		// 2016:057
	{ "F_CSJID",			printsub_csjid },			// 2016:072
	{ "F_JUDGE_INITIALS",		printsub_judge_initials },
	{ "F_INREG_RECEIVER",		printsub_inreg_receiver },	// 2016:082
	{ "F_BANK_ACCOUNTS",		printsub_bank_accounts },
	{ "F_EXECUTION",			printsub_execution },			// 2016:088
	{ "F_EXEC_SUMS_ACCOUNTS",	printsub_exec_sums_accounts },	// 2016:089
	{ "F_EXEC_SUMS_TOTAL",		printsub_exec_sums_total },		// 2016:089
#if COURT_TYPE == COURT_AREAL
	// 2016:180
	{ "F_MEETING_TYPE",		printsub_meeting_type },
	{ "F_TARGET_DEBTOR",		printsub_target_debtor },
	{ "F_TARGET_BULSTAT",		printsub_target_bulstat },
	{ "F_MEETING_AGENDA",		printsub_meeting_agenda },
#endif  // AREAL
	{ "F_FIRST_SET_NO_YEAR",	printsub_first_set_no_year },
	{ "F_INREG_RESULT",		printsub_session_result },
	{ "F_INREG_DATE",			printsub_inreg_date },
	{ "F_INREG_SENDER",		printsub_inreg_sender },
	{ "F_COLLEGE_INITIALS",		printsub_college_initials },
	{ "F_JUDGE_NAME", 		printsub_judge_name },
	{ "F_VALID_DATE",			printsub_valid_date },	// 2017:213
	// 2017:233
	{ "F_REGARD_ADDR_SKIP_UCLP",	printsub_regard_addr_skip_uclp },
	{ "F_REGARD_ADDR_UCLP_ONLY",	printsub_regard_addr_uclp_only },
	{ "F_COURT_ADDR_SKIP_UCLP",	generic_court_addr_skip_uclp },
	{ NULL,				NULL }
};

#include "TranscriptComboBox.h"
#include "TranscriptComboBox.cpp"

#include "AdditionalTextDialog.h"
#include "AdditionalTextDialog.cpp"

#include "LegalAidSideListWindow.h"
#include "LegalAidSideListWindow.cpp"

#include "LegalAidDialog.h"
#include "LegalAidDialog.cpp"

bool print_subpoena(TNotifying *notifying, const char *name, TSubpoenaVariantGroup *variant, bool edit)
{
	TWindow *parent = notifying->pParent;
	TSubpoena *subpoena = &notifying->subpoena;
	TRCDKeyContainer *container = notifying->pKeyContainer;
	TPrintSubpoena printsub(parent, subpoena, container, &variant->links, &variant->laids, notifying->rSentSubpoena,
		variant->allSides, variant->categories, variant->receiver);
	sign aid_exist = false;
	bool lawPt329a3 = false;  // printsub.lawFlagsY & LAWSUIT_Y_PT329A3;  // 2018-09-03: disabled

	printsub.execText = (TText *) variant->sideWind->Find("F_EXEC_TEXT");
	printsub.unwanted = variant->sideWind->unwanted;
	SubpoenaKind->Seek(printsub.subpoenaKind);	// 2009:213 +related
	// 2013:337
	if (strchr(SUBPOENA_KIND_LEGAL_AIDS, printsub.subpoenaKind))
	{
		if (XFER_LEGAL_AID)
		{
			if (printsub.key.type == TYPE_REQUEST)
			{
				info("Искането е по входящ документ. Ще бъдат генерирани отделни искания за всяко лице, без "
					"електронен обмен с програма \"Служебни защити\".");
			}
			else if (printsub.XferDuplicate() || (aid_exist = printsub.XferCollectAll()) == -1)
				return false;
		}

		if (TLegalAidDialog(parent, &printsub).Execute() == IDCANCEL)
			return false;

		variant->OppositeLawers(printsub.otherAuthorizeLawer);
	}
	else if (strchr(SUBPOENA_KIND_EXEC_LISTS, printsub.subpoenaKind))
	{
		bool edit = variant->execList;
		TExecList *execList;

		if (!edit)
			variant->execList = new TExecList;

		execList = variant->execList;
		*execList << subpoena;
		execList->execListYear = printsub.subpoenaDate.year;
		execList->execListKind = printsub.subpoenaKind == SUBPOENA_KIND_EXEC_COUNTRY ? EXEC_LIST_COUNTRY :
			EXEC_LIST_PRIVATE;
		execList->execListDate = printsub.subpoenaDate;

		if (!process_exec_list(parent, execList, variant->links, printsub.sideWinds, variant->categories,
			variant->category, edit))
		{
			return false;
		}

		if (SubpoenaKind->AdditionalText())
		{
			if (PRINT_EXEC_LIST_SUMS && execList->sums.Count())
			{
				emit_exec_list_additional_text(printsub.additionalText, execList, variant->links);
				if (TAdditionalTextDialog(parent, &printsub, container).Execute() == IDCANCEL)
					return false;
				strrep(ncstr(printsub.additionalText), '\n', '\v');
			}
			else
			{
				TOffice::Emit(printsub.additionalText, printsub_fields, &printsub, str(SubpoenaKind->additionalText));
				if (TAdditionalTextDialog(parent, &printsub, container).Execute() == IDCANCEL)
					return false;
			}
		}

		if (!execList->Save(edit))
			return false;

		if (edit)
		{
			mstr m;
			m.printf("на ИЛ%s %ld - %ld/%ld", execList->execListKind == EXEC_LIST_COUNTRY ? " Държ." : "",
				printsub.subpoenaNo, execList->execListNo, execList->execListYear);
			log(LOG_EDIT, 0, container, str(m));
		}

		printsub.execListNo = execList->execListNo;
		printsub.execListKind = execList->execListKind;
		printsub.recipient = execList->recipient;
	}
	// 2010:176 LRQ: surroundKinds
	else if (SubpoenaKind->AdditionalText() || SubpoenaKind->AppealAllowed() ||
		 any(SubpoenaKind->surroundKinds) || SubpoenaKind->HasAccomply() || lawPt329a3)
	{
		// 2016:272
		if (strchr(SUBPOENA_KIND_COUNTEX_EXECLETS, printsub.subpoenaKind))
		{
			TExecList execList;

			execList << printsub.sentSubpoena;
			execList.execListYear = printsub.sentSubpoena->subpoenaDate.year;
			execList.execListKind = printsub.sentSubpoena->subpoenaKind == SUBPOENA_KIND_EXEC_COUNTRY ?
				EXEC_LIST_COUNTRY : EXEC_LIST_PRIVATE;

			if (execList.Get())
			{
				load_exec_list_data(&execList);
				emit_exec_list_additional_text(printsub.additionalText, &execList, variant->links);
				strrep(ncstr(printsub.additionalText), '\n', '\v');
			}
		}

		static constant PT329A3_PREFIX = "\ab\bДелото е по чл.329, ал.3 ЗСВ. "
			"На основание чл.61, ал.2 ГПК сроковете не спират да текат през съдебната ваканция.\a/b\b\v";

		if (lawPt329a3)
			mbk_insert(&printsub.additionalText, PT329A3_PREFIX, 0, strlen(PT329A3_PREFIX));

		TOffice::Emit(printsub.additionalText, printsub_fields, &printsub, str(SubpoenaKind->additionalText));
		if (TAdditionalTextDialog(parent, &printsub, container).Execute() == IDCANCEL)
			return false;
	}

	// 2013:344; 2014:055 // && printsub.eventNo
	if (aid_exist)
	{
		printsub.XferLegalAid();	// 2013:344

		if (!printsub.requestId)
			return false;
	}

	// 2017:065 LRQ: don't print subpoenas with past date
	// 2017:214 LPR: always print subpoenas with valid date
	bool printSaveSubpoena = printsub.subpoenaDate >= Today || !printsub.validUntil.Empty();

	// 2013:084 LPR/LRQ: support for subpoena electrics
	// 2015:222 LPR: insert and copy quickly after Open
	char target[SIZE_OF_PATH];
	TOffice office;
	TWaitWindow wait(parent, WAIT_MORE);
	int retry;

	for (retry = 0; retry < 3; retry++)
	{
		if (printSaveSubpoena)
		{
			if (SAVE_SUBPOENAS)
			{
				// 2015:219 LPR: must be known for target
				// 2016:034 IRQ/FIX: but not for edit
				if (!edit)
					subpoena->Generate();

				electric_name(subpoena, target, TYPE_TEXT);
			}

			if (!office.Open(name, OFFICE_COPY_ALL, SAVE_SUBPOENAS ? basename(target) : NULL))
				return false;

			office.Emit(NULL, NULL, printsub_fields, &printsub);
			office.Close();
		}
		// 2015:252 LPR: FIX: barcode-or-legalaid
		subpoena->requestId = printsub.requestId;
		// 2016:034 LPR: retry update makes no sense
		if (notifying->Insert(variant, edit) || edit)
			break;
	}

	if (retry < 3)
	{
		if (printSaveSubpoena)
		{
			office.ShowSync(&wait, -SAVE_SUBPOENAS, "Призовката/съобщението ще бъде отворено",
				"Призовката/съобщението е записано");

			if (SAVE_SUBPOENAS)
			{
				electric_copy(office.Name, target);
				wait.ResetForeground();
			}
		}

		return true;
	}

	if (variant->execList)
		variant->execList->Delete(true);

	return false;
}
