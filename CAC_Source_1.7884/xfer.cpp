#include "cac.h"

#include "xferc.cpp"

// ----- TXferKind -----------------------------------------------------------
TXferKind::TXferKind()
	: TLongAliasGroup("T_XFER_KIND", TXferKind::Clone, "F_XFER_KIND", &xferKind, "F_NAME", name, sizeof name,
		new TCharAlias("F_CONNECT_KIND", &connectKind), NULL)
{
	Add(
		new TLong("F_TYPES", &types),
		new TLong("F_EISPP", &eispp),
		NULL
	);

	cache = Cache + CACHE_XFER_KIND;
	descs = Descs + TABLE_XFER_KIND;
}

TGroup *TXferKind::Clone()
{
	return new TXferKind;
}

void TXferKind::FloatCriteria(msql &m, const char *fields)
{
	TLongAliasGroup::FloatCriteria(m, fields);
	m.AddRange("(F_XFER_KIND", XFER_CONNECT_FIRST, XFER_CONNECT_LAST);
	m.NextOr();
	m.AddRange("F_XFER_KIND", XFER_EIS_LAW_FIRST, XFER_EIS_LAW_LAST);
	m.cat(")");
	m.AddLike("NOT F_NAME", "дело по несъстоятелност", true);
	m.AddOrder("F_XFER_KIND");
}

// ----- XFER_KIND_ ----------------------------------------------------------
long TXferKind::XferKindRemap()
{
	switch (xferKind)
	{
		case 2027 : return 2010;
		case 2028 : return 2014;
		case 2029 : return 2012;
		case 2030 : return 2016;
	}

	return xferKind;
}

TXferKind *XferKind = NULL;

// ----- TXferCitizen --------------------------------------------------------
bool TXferCitizen::Init(const char *ucn, char ucnType)
{
	TCitizen citizen;

	citizen.ucnType = ucnType;
	strcpy(citizen.ucn, ucn);
	if (citizen.Get())
	{
		strcpy(name, citizen.name);
		strcpy(rename, citizen.reName);
		strcpy(family1, citizen.family);
		strcpy(family2, citizen.reFamily);
		strcpy(egn, ucnType == UCN_CITIZEN_UCN ? citizen.ucn : "");

		return true;
	}

	return false;
}

// ----- TXferTable ----------------------------------------------------------
TXferTable::TXferTable(const char *name, TGroup *(*const tClone)())
	: TGroup(name, tClone)
{
	Add(
		new TLong("F_XFER_KIND", &key.xferKind),
		new TLong("F_NO", &key.no),
		new TLong("F_YEAR", &key.year),
		new TLong("F_COURT_SENDER", &key.courtSender),
		NULL
	);

	descs = Descs + TABLE_XFER_DOCUMENT;
}

void TXferTable::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_XFER_KIND, F_NO, F_YEAR, F_COURT_SENDER");
}

void TXferTable::FloatCriteria(msql &m, const char *fields)
{
	TXferTable::ExactCriteria(m, fields);
}

#define xferTable OFFICE_GROUP_CAST(TXferTable)

static void xfer_xfer_kind(mstr &m, TGroup *group)
{
	xferKind2Kind(xferTable->key.xferKind);
	m.cpy(Kind->name);
}

static void xfer_xfer_alias(mstr &m, TGroup *group)
{
	xferKind2Kind(xferTable->key.xferKind);
	m.cpy(Kind->alias);
}

static void xferweb_no(mstr &m, TGroup *group) { itom(xferTable->key.no, m); }
static void xferweb_year(mstr &m, TGroup *group) { itom(xferTable->key.year, m); }
static void xfer_no_year(mstr &m, TGroup *group) { m.printf("%6ld/%4ld", xferTable->key.no, xferTable->key.year); }
static void xfer_court(mstr &m, TGroup *group) { m.cpy(Court->Seek(xferTable->key.courtSender)); }

#undef xferTable

#define xferDoc OFFICE_GROUP_CAST(TXferDocument)

static void xfer_outreg(mstr &m, TGroup *group) { m.printf("%6ld/%4ld", xferDoc->outreg.no, xferDoc->outreg.year); }

#undef xferDoc

// ----- TXferSide -----------------------------------------------------------
TXferSide::TXferSide(const char *name, TGroup *(*const tClone)())
	: TXferTable(name, tClone)
{
	Add(
		NEW_XFERKEY("F_APPEAL_", &appeal),
		new TLong("F_SIDE_INVOLVEMENT", &sideInvolvement),
		new TLong("F_SIDE_ID", &sideId),
		new TString("F_UCN", ucn, sizeof ucn),
		new TCharAlias("F_UCN_TYPE", &ucnType),
		new TCharAlias("F_INVOLVEMENT", &involvement),
		NULL
	);
}

void TXferSide::ExactCriteria(msql &m, const char *fields)
{
	TXferTable::ExactCriteria(m, fields);
	m.Add(this, "F_SIDE_ID");
}

void TXferSide::Init(const char *involvements)
{
	msql m("T_INVOLVEMENT", "F_INVOLVEMENT");

	involvement = INVOLVEMENT_OTHER;
	m.AddLong("F_XFER", sideInvolvement);

	TQuery q(str(m));
	if (q.Read())
	{
		*Involvement << q;
		if (strchr(involvements, Involvement->involvement))
			involvement = Involvement->involvement;
	}
}

void TXferSide::Load(const char *involvements)
{
#if RANGECHECK
	if (ucnType)
		fatal("xfer side Load() recalled");
#endif  // RANGECHECK

	if (involvements)
		Init(involvements);
}

void TXferSide::Print(mstr &m)
{
	m.cat(Involvement->Seek(involvement));
	m.setlen(SIZE_OF_INVOLVEMENT_DISPLAY);
}

// ----- TXferSideCitizen ----------------------------------------------------
TXferSideCitizen::TXferSideCitizen()
	: TXferSide("T_XFER_SIDE_CITIZEN", TXferSideCitizen::Clone)
{
	Add(
		NEW_XFER_CITIZEN("F_SIDE_", &side),
		NULL
	);

	descs = Descs + TABLE_XFER_SIDE_CITIZEN;
}

TGroup *TXferSideCitizen::Clone()
{
	return new TXferSideCitizen;
}

void TXferSideCitizen::Load(const char *involvements)
{
	TCitizen citizen;

	TXferSide::Load(involvements);
	strcpy(citizen.name, strupr(side.name));
	strcpy(citizen.reName, strupr(side.rename));
	strcpy(citizen.family, strupr(side.family1));
	strcpy(citizen.reFamily, strupr(side.family2));

	if (atob(side.egn) && ucn_valid(UCN_CITIZEN_UCN, side.egn))
	{
		citizen.ucnType = UCN_CITIZEN_UCN;
		strcpy(citizen.ucn, side.egn);

		if (citizen.Try() || citizen.Insert())
		{
			ucnType = UCN_CITIZEN_UCN;
			strcpy(ucn, side.egn);
			Update("F_UCN_TYPE, F_UCN");
		}
	}
	else
	{
		citizen.ucnType = UCN_CITIZEN_CODE;
		*citizen.ucn = '\0';

		if (citizen.Insert())
		{
			ucnType = UCN_CITIZEN_CODE;
			strcpy(ucn, citizen.ucn);
			Update("F_UCN_TYPE, F_UCN");
		}
	}
}

void TXferSideCitizen::Print(mstr &m)
{
	TXferSide::Print(m);

	if (any(side.egn) && ucn_valid(UCN_CITIZEN_UCN, side.egn))
		m.printf("ЕГН %-10s  ", side.egn);
	else
		m.printf("%15s ", "");

	m.printf("%s %s %s %s", side.name, side.rename, side.family1, side.family2);
}

// ----- TXferSideFirm -------------------------------------------------------
TXferSideFirm::TXferSideFirm()
	: TXferSide("T_XFER_SIDE_FIRM", TXferSideFirm::Clone)
{
	Add(
		new TString("F_SIDE_NAME", sideName, sizeof sideName),
		new TString("F_SIDE_BULSTAT", sideBulStat, sizeof sideBulStat),
		NULL
	);

	descs = Descs + TABLE_XFER_SIDE_FIRM;
}

TGroup *TXferSideFirm::Clone()
{
	return new TXferSideFirm;
}

void TXferSideFirm::Load(const char *involvements)
{
	TFirm firm;

	TXferSide::Load(involvements);

	firm.ucnType = UCN_FIRM_CODE;
	*firm.ucn = '\0';
	strcpy(firm.name, strupr(sideName));
	strcpy(firm.bulstat, sideBulStat);

	// 2011:139 LRQ: find by bulstat+name
	if (any(firm.bulstat))
	{
		msql m;

		m.Begin(firm.Name);
		m.AddString("F_BULSTAT", firm.bulstat);

		TQuery q(str(m));
		while (q.Read())
		{
			TFirm tFirm;

			tFirm << q;
			if (similar_firname(&firm, &tFirm))
			{
				firm << tFirm;
				break;
			}
		}
	}

	if (any(firm.ucn) || firm.Insert())
	{
		ucnType = firm.ucnType;
		strcpy(ucn, firm.ucn);
		Update("F_UCN_TYPE, F_UCN");
	}
}

void TXferSideFirm::Print(mstr &m)
{
	TXferSide::Print(m);
	m.printf("%15s %s", sideBulStat, sideName);
}

// ----- TXferConnect --------------------------------------------------------
bool TXferConnectData::Init(const TConnect &connect)
{
	if (!connect.connectKind)
		return false;

	connectType = ConnectType->Xfer(connect.connectType);
	connectKind = ConnectKind->Xfer(connect.connectKind);
	connectNo = atof(connect.connectNo);
	connectYear = connect.connectYear;
	return (connectCourt = sender2Court(connect.ucn, connect.ucnType)) != 0;
}

TXferConnect::TXferConnect()
	: TXferTable("T_XFER_CONNECT", TXferConnect::Clone)
{
	Add(
		NEW_XFER_CONNECT("CONNECT", this),
		NULL
	);

	descs = Descs + TABLE_XFER_CONNECT;
}

TGroup *TXferConnect::Clone()
{
	return new TXferConnect;
}

bool TXferConnect::Init(const TXferDocument *document)
{
	key = document->key;
	return connect.TryBy(document->container, CONNECT_TYPE_LOWER_INSTANCE) && TXferConnectData::Init(connect);
}

bool TXferConnect::Init(const TXferDocument *document, const TOutReg *outReg)
{
	key = document->key;
	return outReg->TryLower(connect) && TXferConnectData::Init(connect);
}

// ----- TXferAppeal ---------------------------------------------------------
TXferAppeal::TXferAppeal()
	: TXferTable("T_XFER_APPEAL", TXferAppeal::Clone)
{
	Add(
		NEW_XFERKEY("F_APPEAL_", &appeal),
		new TString("F_APPEAL_FILE", appealFile, sizeof appealFile),
		NULL
	);

	descs = Descs + TABLE_XFER_APPEAL;
}

TGroup *TXferAppeal::Clone()
{
	return new TXferAppeal;
}

bool TXferAppeal::Init(TInReg *inReg)
{
	appeal.xferKind = Kind->Xfer(inReg->kind);
	appeal.no = inReg->key.no;
	appeal.year = inReg->key.year;
	xfer_name(inReg, appealFile, true);
	return true;
}

// ----- TXferDocument -------------------------------------------------------
TXferDocument::TXferDocument()
	: TXferTable("T_XFER_DOCUMENT", TXferDocument::Clone), container(NULL)
{
	Add(
		new TString("F_EISPP_NO", eisppNo, sizeof eisppNo),
		NEW_XFERKEY("F_INDOC_", &indoc),
		new TString("F_INDOC_FILE", indocFile, sizeof indocFile),
		NEW_XFERKEY("F_OUTREG_", &outreg),
		new TString("F_RETURN_RESULT", returnResult, sizeof returnResult),
		new TLong("F_COURT_RECEIVER", &courtReceiver),
		new TChar("F_XFER_TYPE", &xferType),
		new TLong("F_IO", &io),
		NULL
	);

	io = XFER_INVALID;
}

TXferDocument::~TXferDocument()
{
	delete container;
}

TGroup *TXferDocument::Clone()
{
	return new TXferDocument;
}

void TXferDocument::Accept(const TRequest *request)
{
	if (appeals.Count())
	{
		TXferAppeal *appeal = appeals[0];

		if (appeal->NonMatchingFile())
			xfer_file(request, appeal->appealFile);

		mstr involvements;

		for (int index = 1; index < appeals.Count(); index++)
		{
			appeal = appeals[index];
			TSurround surround;

			if (XFER_LINK_APPEALS)
			{
				TElectric electric;

				electric << request;
				electric.extraNo = SUBPOENA_NO_MAX + index;
				sprintf(electric.descript, "%s %ld/%ldг.", XferKind->Seek(appeal->appeal.xferKind),
					appeal->appeal.no, appeal->appeal.year);

				if (xfer_file(request, appeal->appealFile, electric.extraNo))
					electric.Insert();
			}
			else if (CreateSurround(request, &surround, XferKind->Seek(appeal->appeal.xferKind)))
			{
				if (!any(involvements))
					surround.Involvements(involvements);

				TXferSideCitizen citizen;
				TXferSideFirm xferFirm;

				xfer_file(&surround, appeal->appealFile);
				AutoAppealSides(&citizen, appeal, &surround, str(involvements));
				AutoAppealSides(&xferFirm, appeal, &surround, str(involvements));
			}
		}
	}

	// 2014:147
	TXferDecision decision;
	decision.key = key;
	// 2014:301 FIX: Get() -> Try()
	// 2015:152 LRQ: non-matching decision file support
	if (decision.Try("F_DECISION_FILE, F_MOTIVES_FILE"))
	{
		if (any(decision.decisionFile))
		{
			for (int type = 0; type < TROUBLE_COUNT; type++)
			{
				if (electric_matches(decision.decisionFile, type, true, false))
				{
					if (!loadMatch(type, decision.decisionFile))
						xfer_file(request, decision.decisionFile);
					break;
				}
			}
		}

		if (any(decision.motivesFile))
			xfer_file(request, decision.motivesFile, EXTRA_NO_XFER_MOTIVES);
	}
	else if (NonMatchingFile(indocFile))
		xfer_file(request, indocFile);  // 2017:229 non-matching indoc file support
}

bool TXferDocument::CreateSurround(const TRequest *request, TSurround *surround, const char *what)
{
	surround->key.year = request->key.year;
	surround->kind = KIND_ANOTHER_SURROUND;
	surround->date = request->date;
	surround->college = request->college;
	surround->lawsuit = request->key;
	sprintf(surround->text, "%s по ", what);

	mstr m;
	primary.Print(m);
	strcat(surround->text, str(m));

	*surround->judge = '\0';		// 2011:304 was EMPTY
	surround->time = request->time;
	surround->xfer = true;

	if (surround->Insert())
		return true;

	LogAny("грешка %s запис", surround->Name);
	return false;
}

void TXferDocument::AutoAppealSides(TXferSide *side, TXferAppeal *appeal, TSurround *surround, const char *involvements)
{
	msql m;

	side->key = appeal->key;
	side->FloatCriteria(m);
	m.Add(appeal, "F_APPEAL_KIND, F_APPEAL_NO, F_APPEAL_YEAR");
	TQuery q(str(m));

	while (q.Read())
	{
		*side << q;
		bool appealer = side->ucnType != '\0';	// (?)

		// 2011:129 LPR: don't reload
		if (!side->ucnType)
			side->Load(involvements);
		// 2015:037 loaded side involvements are not saved
		else if (!side->involvement)
			side->Init(involvements);

		if (!appealer)
			appealer = side->involvement == INVOLVEMENT_INAPPEALER;

		TSideWind sideWind;

		sideWind << side;
		sideWind << surround;
		if (appealer)
			sideWind.state = STATE_SURROUND;
		if (!sideWind.Insert())
			LogAny("грешка %s запис", sideWind.Name);
	}
}

bool TXferDocument::Delete(bool exact)
{
	return TXferTable::Delete(exact);
}

bool TXferDocument::Delete()
{
	struct TXferDelete
	{
		const char *name;
		const char *fileName;
		bool remove;
	};

	static const TXferDelete xferDelete[] =
	{
		{ "T_XFER_DOCUMENT", "F_INDOC_FILE", true },
		{ "T_XFER_CONNECT", NULL, true },
		{ "T_XFER_DECISION", "F_DECISION_FILE", false },
		{ "T_XFER_DECISION", "F_MOTIVES_FILE", true },
		{ "T_XFER_APPEAL", "F_APPEAL_FILE", true },
		{ "T_XFER_SIDE_CITIZEN", NULL, true },
		{ "T_XFER_SIDE_FIRM", NULL, true },
		{ NULL, false }
	};

	bool first = true;

	for (const TXferDelete *xdel = xferDelete; xdel->name; xdel++)
	{
		TXferTable xtable(xdel->name, NULL);
		msql m;

		xtable.key = key;

		if (io == XFER_RECEIVE && xdel->fileName)
		{
			char fileName[SIZE_OF_XFER_PATH];

			xtable.Add(new TString(xdel->fileName, fileName, sizeof fileName));

			if (first)
				xtable.ExactCriteria(m);
			else
				xtable.FloatCriteria(m);

			TQuery q(str(m));
			while (q.Read())
			{
				xtable << q;

				if (any(fileName) && !unlock_delete_file(fileName, true))
					LogAny("грешка %s изтриване", basename(fileName));
			}
		}

		if (xdel->remove && !xtable.Delete(first))
		{
			LogAny("грешка %s изтриване", xtable.Name);
			if (first)
				break;
		}

		first = false;
	}

	return !first;
}

void TXferDocument::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.AddLong("F_IO", XFER_RECEIVE);
	m.AddChars("F_XFER_TYPE", XFER_TYPE_INCOMINGS);
}

bool TXferDocument::Init(const TOutReg *outReg)
{
	container = type2RCDKeyContainer(outReg->lawsuit.type);
	container->key = outReg->lawsuit;

	if (!container->Get(outReg->lawsuit.type == TYPE_REQUEST ? "F_KIND, F_DATE, F_EISPP_NMR" :
		"F_KIND, F_DATE, F_SOURCE_TYPE, F_SOURCE_KIND, F_SOURCE_NO, F_SOURCE_YEAR, F_EISPP_NMR"))
		return false;

	key.xferKind = Kind->Xfer(container->kind);
	key.no = flag_key_no(&outReg->lawsuit);
	key.year = outReg->lawsuit.year;
	key.courtSender = COURT_CODE;

	if (outReg->lawsuit.type == TYPE_REQUEST)
	{
		TRequest *request = (TRequest *) container;

		requestKind = request->kind;
		strcpy(eisppNo, request->eispp_nmr);	// 2012:307
		// 2009:223 FIX: was missing
		if (outReg->receiverType == UCN_INSTANCE_EQUAL || outReg->receiverType == UCN_INSTANCE_CONFLICT)
			xfer_name(request, indocFile, true);
	}
	else
	{
		TLawsuit *lawsuit = (TLawsuit *) container;

		requestKind = lawsuit->sourceKind;
		strcpy(eisppNo, lawsuit->eispp_nmr);	// 2012:307

		if (outReg->receiverType == UCN_INSTANCE_EQUAL || outReg->receiverType == UCN_INSTANCE_CONFLICT)
		{
			indoc.xferKind = Kind->Xfer(requestKind);

			if (lawsuit->source.type == TYPE_REQUEST)
			{
				TRequest request;

				request.key = lawsuit->source;
				request.kind = requestKind;	// 2009:206 FIX: was missing

				if (!request.Get("F_DATE"))
					return false;

				indoc.no = lawsuit->source.no;
				indoc.year = lawsuit->source.year;
				xfer_name(&request, indocFile, true);
			}
		}
	}

	if (outReg->receiverType == UCN_INSTANCE_LOWER)
	{
		if (!connect.Init(this, outReg))
			return false;

		connect.connectType = ConnectType->Xfer(connect.connect.connectType);
		TConnect &connect = TXferDocument::connect.connect;

		// 2009:197 FIX: ConnectKind -> Kind; 2009:206 FIX: zero-check
		outreg.xferKind = Kind->Xfer(connect.outKind ? connect.outKind : KIND_OUT_LETTER);
		outreg.no = connect.outNo;
		outreg.year = connect.outYear;
	}
	else
	{
		if (outReg->receiverType == UCN_INSTANCE_EQUAL)
			outreg.xferKind = XFER_OUT_LETTER_EQUAL;
		else if (outReg->receiverType == UCN_INSTANCE_COMPETENT)
			outreg.xferKind = XFER_OUT_LETTER_COMPETENT;
		else
			outreg.xferKind = Kind->Xfer(outReg->kind);

		outreg.no = outReg->key.no;
		outreg.year = outReg->key.year;
	}

	strcpy(returnResult, outReg->vssIndex);
	xferType = outReg->receiverType;
	io = XFER_SEND;
	return true;
}

void TXferDocument::Load(TRequest *request, TGArray<TConnect> *connects, TElectricTrouble *trouble)
{
	long indocKind = indoc.xferKind ? indoc.xferKind : key.xferKind;
	TConnect *connect;

	if (xferType == UCN_INSTANCE_EQUAL)
	{
		request->kind = xferKind2indocKind(indocKind, request->college);
		load_names(trouble, indocFile);
	}
	else if (xferType == UCN_INSTANCE_COMPETENT)
	{
		TXferConnect tConnect;
		tConnect.key = key;

		if (tConnect.Get())
		{
			msql m(ConnectType->Name);

			m.AddLong("F_XFER", tConnect.connectType);
		#if 20170230
			if (ConnectType->Get(m))
			{
				connect = new TConnect;
				connect->key = request->key;
				connect->ucnType = ConnectType->senderType;
				xfer2sender(connect->ucn, connect->ucnType, tConnect.connectCourt);
				if (tConnect.connectNo)
					itoa(tConnect.connectNo, connect->connectNo, 10);
				connect->connectYear = tConnect.connectYear;
				connect->connectType = ConnectType->connectType;
				m.Begin(ConnectKind->Name);
				m.AddLong("F_XFER", tConnect.connectKind);

				if (ConnectKind->Get(m))
				{
					connect->connectKind = ConnectKind->connectKind;
					// 2013:051 FIX: comout, we don't support non-eispp xfer kinds
					//connect->xferKind = XferKind->XferKindRemap();
				}

				connects->Add(connect);
			}
			else
				say_no_data(ConnectType->Name);
		#else  // 20170230
			TQuery q(str(m));
			if (q.Read())
			{
				*ConnectType << q;
				connect = new TConnect;

				connect->key = request->key;
				connect->ucnType = ConnectType->senderType;
				xfer2sender(connect->ucn, connect->ucnType, tConnect.connectCourt);
				if (tConnect.connectNo)
					itoa(tConnect.connectNo, connect->connectNo, 10);
				connect->connectYear = tConnect.connectYear;
				connect->connectType = ConnectType->connectType;
				m.Begin(ConnectKind->Name);
				m.AddLong("F_XFER", tConnect.connectKind);
				TQuery r(str(m));
				if (r.Read())
				{
					*ConnectKind << r;
					connect->connectKind = ConnectKind->connectKind;
					// 2013:051 FIX: comout, we don't support non-eispp xfer kinds
					//connect->xferKind = XferKind->XferKindRemap();
				}
				else
					say_no_data(ConnectKind->Name);

				connects->Add(connect);
			}
			else
				say_no_data(ConnectType->Name);
		#endif  // 20170230
		}
	}

	connect = new TConnect;
	connect->key = request->key;

	switch (xferType)
	{
		// 2009:258 LPR: high jurisdiction support
#if COURT_TYPE == COURT_APPEAL
		case UCN_INSTANCE_EQUAL : connect->connectType = CONNECT_TYPE_HIGH_JURISDICTION; break;
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
		case UCN_INSTANCE_EQUAL : connect->connectType = strchr(KIND_1ST_INDOCS, request->kind) ? CONNECT_TYPE_JURISDICTION : CONNECT_TYPE_HIGH_JURISDICTION; break;
#endif  // ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
		case UCN_INSTANCE_EQUAL : connect->connectType = CONNECT_TYPE_JURISDICTION; break;
#endif  // REGIONAL
		case UCN_INSTANCE_COMPETENT : connect->connectType = CONNECT_TYPE_COMPETENT_HIGH; break;
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
		case UCN_INSTANCE_CONFLICT : connect->connectType = CONNECT_TYPE_LOWER_INSTANCE; break;
		case UCN_INSTANCE_HIGHER : connect->connectType = CONNECT_TYPE_LOWER_INSTANCE; break;
#endif  // APPEAL || ADMIN || AREAL || MILITARY
		default : fatal("%d: xfer type not supported", xferType);
	}

	ConnectType->Seek(connect->connectType);
	connect->ucnType = ConnectType->senderType;

	xfer2sender(connect->ucn, connect->ucnType, key.courtSender);
	itoa(key.no, connect->connectNo, 10);
	connect->connectYear = key.year;

	XferKind->Seek(key.xferKind);
	connect->connectKind = XferKind->connectKind;
	connect->xferKind = XferKind->XferKindRemap();
	if ((connect->outKind = xferKind2Kind(outreg.xferKind, TYPE_OUTREG)) == '\0')
		connect->outKind = KIND_OUT_LETTER;
	connect->outNo = outreg.no;
	connect->outYear = outreg.year;

	if (xferType == UCN_INSTANCE_HIGHER)
	{
		TXferAppeal tAppeal;
		TXferDecision decision;
		msql m;

		tAppeal.key = key;
		tAppeal.FloatCriteria(m);
		m.AddOrder("F_APPEAL_YEAR, F_APPEAL_NO");
		TQuery q(str(m));

		while (q.Read())
		{
			TXferAppeal *appeal = new TXferAppeal;
			*appeal << q;

			if (!appeals.Count())
			{
				if (appeal->NonMatchingFile())
					electric_action(appeal->appealFile, TYPE_IMAGE, ACTION_VIEW);

				xfer_indoc_kind(request, key.xferKind, appeal->appeal.xferKind, request->college);
				load_names(trouble, appeal->appealFile);
			}

			appeals.Add(appeal);
		}

		if (!appeals.Count())
		{
			error("Липсва информация за документите за обжалване!\n"
				"\nНеобходимо е да изберете вид на входящия документ.");
		}

		decision.key = key;

		if (decision.Get())
		{
			connect->decisionKind = xferKind2Kind(decision.decisionKind);
			connect->decisionDate = decision.decisionDate;
		}

		load_names(trouble, decision.decisionFile);
	}

	connects->Add(connect);
	primary << connect;

	// 2012:307 rewritten; 2012:342 length check; 2014:164 db len fixed
	strcpy(request->eispp_nmr, eisppNo);
}

void TXferDocument::LogAny(const char *what, const char *text, char gop)
{
	mstr m("xfer:");
	m.printf(what, text);
	log(gop, 0, key.no, key.year, TYPE_TRANSFER, KIND_NULL, NULL, str(m));
}

void TXferDocument::Print(mstr &m)
{
	XferKind->Seek(key.xferKind);
	ConnectKind->Seek(XferKind->connectKind);
	m.printf("%-16s  %-8s  %6ld/%4ld  %s", xfer_type_name(xferType), ConnectKind->alias, key.no, key.year,
		Court->Seek(key.courtSender));
}

// ----- TXferDecision -------------------------------------------------------
TXferDecision::TXferDecision()
	: TXferTable("T_XFER_DECISION", TXferDecision::Clone)
{
	Add(
		new TLong("F_DECISION_KIND", &decisionKind),
		new TLong("F_DECISION_NO", &decisionNo),
		new TXferDate("F_DECISION_DATE", &decisionDate),
		new TString("F_DECISION_FILE", decisionFile, sizeof decisionFile),
		NEW_XFER_CITIZEN("F_JUDGE1_", &judge[0]),
		NEW_XFER_CITIZEN("F_JUDGE2_", &judge[1]),
		NEW_XFER_CITIZEN("F_JUDGE3_", &judge[2]),
		new TString("F_MOTIVES_FILE", motivesFile, sizeof motivesFile),
		NULL
	);

	descs = Descs + TABLE_XFER_DECISION;
}

TGroup *TXferDecision::Clone()
{
	return new TXferDecision;
}

bool TXferDecision::Init(const TInReg *inReg, bool motivable)
{
	if (inReg->sessionKind && strchr(KIND_SESSIONS, inReg->sessionKind))
	{
		TSession session;

		session.key = inReg->lawsuit;
		session.kind = inReg->sessionKind;
		session.date = inReg->decisionDate;

		if (session.Get("F_JUDGE, F_JUDGE_X, F_REJUDGE_X, F_PRESIDENT, F_EVENT_NO"))
		{
			TDecision decision;

			// 2014:343 FIX: decisionKind is not always endoc kind
			decisionKind = Kind->Xfer(strchr(KIND_ENDOCS, inReg->decisionKind) ? inReg->decisionKind : KIND_ORDER);
			decision << session;
			decision.kind = inReg->decisionKind;

			// 2016:176 LPR: extraNo support
			if (inReg->extraNo)
			{
				TSurroundment *const surrouendoc = (TSurroundment *) inReg->AppealedAct(false, false);

				if (!surrouendoc)
					return false;

				TVirtPtr VP(surrouendoc);
				decisionNo = PRINT_JUDGEMENTS ? surrouendoc->eventNo : 0;
				decisionDate = surrouendoc->decisionDate;

				TDecision decision;
				decision.Init(surrouendoc);
				xfer_name(&decision, decisionFile, true);
			}
			// 2014:147 fill motivesFile, get motives fields
			else if (decision.Try("F_FROM_DATE, F_EVENT_NO, F_MOTIVES, F_FLAGS, F_RETURNED"))
			{
				// 2009:195 LRQ: optional decisionNo
				decisionNo = PRINT_JUDGEMENTS ? decision.eventNo : 0;
				decisionDate = decision.fromDate;
				xfer_name(&decision, decisionFile, true);

				// 2014:181 LPR/LRQ: disabled for now; 2014:206 check
				if (motivable && decision.Motivated())
				{
					TDecisionRider motives(&decision, KIND_MOTIVES);
					xfer_name(&motives, motivesFile, true);
				}
			}
			else
			{
				decisionNo = PRINT_JUDGEMENTS ? session.eventNo : 0;
				decisionDate = inReg->decisionDate;
				xfer_name(&session, decisionFile, true);
			}

			const char *judges[3];
			session2Judges(&session, judges);

			for (int i = 0; i < 3 && atob(judges[i]); i++)
				if (!judge[i].Init(judges[i], UCN_CITIZEN_UCN))
					return false;

			return true;
		}
	}
	// 2009:203 FIX: was missing
	else
	{
		TIndexContainer *container;

		if (inReg->lawsuit.type == TYPE_REQUEST)
		{
			container = new TRequestResolution;
			// must be lawsuit, decision is incomplete
			container->key = inReg->lawsuit;
		}
		else	// pre-checked in xfer_sendable
		{
			container = new TInRegEvent;
			// 2012:165 TRQ: FIX: lawsuit -> decision
			container->key = inReg->decision;
		}

		TVirtPtr VP(container);
		container->date = inReg->decisionDate;

		if (container->Get("F_JUDGE, F_EVENT_NO"))
		{
			TString *tJudge = (TString *) container->Find("F_JUDGE");

			// 2014:343 FIX: decisionKind is not always endoc kind
			decisionKind = Kind->Xfer(strchr(KIND_ENDOCS, inReg->decisionKind) ? inReg->decisionKind : KIND_ORDER);
			decisionNo = PRINT_JUDGEMENTS ? ((TLong *) container->Find("F_EVENT_NO"))->L() : 0;
			decisionDate = inReg->decisionDate;
			xfer_name(container, decisionFile, true);
			judge[0].Init(tJudge->S(), UCN_CITIZEN_UCN);
			return true;
		}
	}

	return false;
}

bool TXferDecision::Init(const TOutReg *outReg)
{
	if (!outReg->decisionKind || outReg->decisionDate.Empty())
		return false;

	TIndexContainer *const container = (TIndexContainer *) kind2DatedContainer(outReg->decisionKind);
	TVirtPtr VP(container);

	container->key = outReg->lawsuit;
	container->date = outReg->decisionDate;
	xfer_name(container, decisionFile, true);

	bool result = container->Get(container->key.type == TYPE_REQUEST ? "F_DATE, F_EVENT_NO" :
		"F_FROM_DATE AS F_DATE, F_EVENT_NO");

	if (result)
	{
		decisionKind = Kind->Xfer(outReg->decisionKind);
		decisionDate = container->date;

		if (PRINT_JUDGEMENTS)
			decisionNo = container->eventNo;
	}

	return result;
}

// ----- TXfer ---------------------------------------------------------------
class TXfer
{
public:
	TXfer(const TOutReg *tOutReg, const TInReg *tInReg);

	bool Delete(const char *what, TXferDocument *document);
	void Send();

protected:
	const TOutReg *outReg;
	const TInReg *inReg;
	long sideId;

	bool Log(const char *what, const char *s);
	bool Log(const char *what, TGroup *group) { return Log(what, group->Name); }
	bool Log(const char *what, TGroup &group) { return Log(what, group.Name); }
	bool WriteSides(TRCDKeyContainer *container, TXferDocument *document, TXferAppeal *appeal);
	bool Send(TXferDocument *document, bool motivable);
};

TXfer::TXfer(const TOutReg *tOutReg, const TInReg *tInReg)
	: outReg(tOutReg), inReg(tInReg), sideId(0)
{
}

bool TXfer::Log(const char *what, const char *s)
{
	mstr m("xfer:");
	m.printf(what, s);
	log(LOG_TRANSFER, 0, outReg, str(m));
	return false;
}

bool TXfer::Delete(const char *what, TXferDocument *document)
{
	if (!document->Init(outReg))
		return Log("грешка %s попълване", document);

	TXferTable xtable(document->Name, NULL);

	xtable.key = document->key;
	if (xtable.Try())
	{
		Log("изтриване на %s", what);
		if (!document->Delete())
			return Log("грешка %s изтриване", document);
	}

	return true;
}

bool TXfer::WriteSides(TRCDKeyContainer *container, TXferDocument *document, TXferAppeal *appeal)
{
	TGArray<TSideWind> sideWinds;
	TXferSideCitizen citizen;
	TXferSideFirm xferFirm;

	load_sidebasicv(container, sideWinds);
	for (int i = 0; i < sideWinds.Count(); i++)
	{
		TSideWind *sideWind = sideWinds[i];
		TXferSide *xferSide;

		if (appeal && !sideWind->Appealer())
			continue;

		if (strchr(UCN_CITIZENS, sideWind->ucnType))
		{
			if (!citizen.side.Init(sideWind->ucn, sideWind->ucnType))
				return Log("грешка %s попълване", citizen);
			xferSide = &citizen;
		}
		else
		{
			TFirm firm;

			firm.ucnType = sideWind->ucnType;
			strcpy(firm.ucn, sideWind->ucn);
			if (!firm.Try())
				return Log("грешка %s попълване", firm);
			strcpy(xferFirm.sideName, firm.name);
			xferFirm.sideName[XFER_FIRM_NAME] = '\0';		// 2009:348
			strcpy(xferFirm.sideBulStat, firm.bulstat);
			xferSide = &xferFirm;
		}

		xferSide->key = document->key;

		if (appeal)
		{
			xferSide->appeal = appeal->appeal;
#if COURT_TYPE == COURT_AREAL
			const char *s = strchr(XFER_FROM_INVOLVEMENTS, sideWind->involvement);
			if (s)
				xferSide->sideInvolvement = Involvement->Xfer(XFER_TO_INVOLVEMENTS[s - XFER_FROM_INVOLVEMENTS]);
			else
#endif  // AREAL
				xferSide->sideInvolvement = Involvement->Xfer(INVOLVEMENT_INAPPEALER);
		}
		else
			xferSide->sideInvolvement = Involvement->Xfer(sideWind->involvement);

		xferSide->sideId = ++sideId;
		if (!xferSide->Insert())
			return Log("грешка %s запис", xferSide);
	}

	return sideId;
}

bool TXfer::Send(TXferDocument *document, bool motivable)
{
	TXferConnect &connect = document->connect;

	if (outReg->receiverType == UCN_INSTANCE_HIGHER)
	{
		TXferDecision decision;

		if (!decision.Init(inReg, motivable))
			return Log("грешка %s попълване", decision);
		decision.key = document->key;
		if (!decision.Insert())
			return Log("грешка %s запис", decision);

		TInRegEvent inRegEvent;
		msql m;

		inRegEvent.OutCriteria(outReg);
		inRegEvent.FloatCriteria(m);
		TQuery q(str(m));
		while (q.Read())
		{
			TInReg inReg;

			inRegEvent << q;
			inReg.key = inRegEvent.key;

			if (!inReg.Get("F_KIND, F_DATE"))
				return Log("грешка %s четене", inReg);

			TXferAppeal appeal;

			if (!appeal.Init(&inReg))
				return Log("грешка %s попълване", appeal);
			appeal.key = document->key;
			if (!appeal.Insert())
				return Log("грешка %s запис", appeal);
			if (!WriteSides(&inReg, document, &appeal))
				return false;
		}
	}
	// 2009:203 LPR: universal non-appeal-send/return sides
	else if (outReg->receiverType != UCN_INSTANCE_LOWER && !WriteSides(document->container, document, NULL))
		return false;

	// 2009:209 LPR: using receiverType directly
	if (outReg->receiverType == UCN_INSTANCE_COMPETENT)
	{
		if (!connect.Init(document))
			return Log("грешка %s попълване", connect);
		if (!connect.connectKind)
			return Log("%s: липсва точен вид дело", connect);
	#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		connect.connectType = XFER_CONNECT_TYPE_STRIFE;
	#endif  // APPEAL || ADMIN || MILITARY || REGIONAL
	#if COURT_TYPE == COURT_AREAL
		connect.connectType = document->requestKind == KIND_INDOC_REJECT_JUDGES ? XFER_CONNECT_TYPE_CONFLICT : XFER_CONNECT_TYPE_STRIFE;
	#endif  // AREAL
		if (!connect.Insert())
			return Log("грешка %s запис", connect);
	}
	else if (outReg->receiverType == UCN_INSTANCE_LOWER)
	{
		TXferDecision decision;

		if (!decision.Init(outReg))
			return Log("грешка %s попълване", decision);
		decision.key = document->key;
		if (!decision.Insert())
			return Log("грешка %s запис", decision);
		// N.B. pre-initialized in TXferDocument::Init
		if (!connect.Insert())
			return Log("грешка %s запис", connect);
	}

	return true;
}

void TXfer::Send()
{
	TXferDocument document;
	TSender sender;

	if (outReg->lawsuit.type && get_receiver(outReg, &sender) && Court->Seek(sender.court))
	{
		if (!strchr(XFER_COURT_TYPES, Court->courtType))
			return;

		bool motivable = Court->flags & COURT_XFER_MOTIVES;
		document.courtReceiver = sender.court;

		if (!Delete("предишни данни", &document))
			Log("грешка %s изтриване/1", document);
		else if (!document.Insert())
			Log("грешка %s запис", document);
		else if (!Send(&document, motivable))
		{
			Log("грешка %s изпращане", document);
			if (!Delete("некомплектните данни", &document))
				Log("грешка %s изтриване/2", document);
		}
		else
			Log("подготвен за изпращане", document);
	}
	else
		Log("грешка %s адресант", document);
}

// ----- functions -----------------------------------------------------------
void xfer_delete(const TOutReg *outReg)
{
	TXferDocument document;
	TXfer(outReg, NULL).Delete("данни за изпращане", &document);
}

static bool xfer_sendable(const TInReg *inReg)
{
	return
		inReg->lawsuit.type == TYPE_REQUEST ||
		inReg->decision.type == TYPE_INREG ||	// 2012:165 TRQ: FIX: lawsuit -> decision
		(inReg->sessionKind && strchr(KIND_SESSIONS, inReg->sessionKind));
}

void xfer_send(const TOutReg *outReg, const TInReg *inReg)
{
	if ((XFER_ACTIVE & XFER_REQLAWS) && (!inReg || xfer_sendable(inReg)))
		TXfer(outReg, inReg).Send();
}

bool xfer_resendable(const TOutReg *outReg, const TInReg *inReg)
{
	return (XFER_ACTIVE & XFER_REQLAWS) && strchr(UCN_RECEIVER_SENDERS, outReg->receiverType) &&
		(!inReg || xfer_sendable(inReg));
}

void xfer_resend(const TOutReg *outReg, const TInReg *inReg)
{
	if (outReg->resend)
		TXfer(outReg, inReg).Send();
}

#include "XferDocumentListBox.h"
#include "XferDocumentListBox.cpp"

#include "XferDocumentDialog.h"
#include "XferDocumentDialog.cpp"

bool xfer_load(TWindow *parent, TRequest *request, TGArray<TConnect> *connects, TElectricTrouble *trouble)
{
	TXferDocument *document = new TXferDocument;

	if (TXferDocumentDialog(parent, document, request).Execute() != IDOK)
	{
		delete document;
		return false;
	}

	document->Load(request, connects, trouble);
	xfer_reject(request);
	request->xfer = document;
	return true;
}

#include "XferSideListBox.h"
#include "XferSideListBox.cpp"

#include "XferSideDialog.h"
#include "XferSideDialog.cpp"

void xfer_load(TWindow *parent, TRequest *request, TGArray<TSideWind> *sideWinds)
{
	TGSArray<TXferSide> xferSides;

	if (TXferSideDialog(parent, request->xfer, request, &xferSides).Execute() == IDOK)
	{
		for (int i = 0; i < xferSides.Count(); i++)
		{
			TXferSide *xferSide = xferSides[i];
			TSideWind *sideWind = new TSideWind;

			// 2011:129 LPR: don't reload
			if (!xferSide->ucnType)
				xferSide->Load(NULL);

			*sideWind << xferSide;
			*sideWind << request;
			sideWinds->Add(sideWind);
		}
	}
}

void xfer_reject(TRequest *request)
{
	delete request->xfer;
	request->xfer = NULL;
}

static bool xfer_delete(TXferDocument *document, const char *what)
{
	return document->Delete() ? true : error("%s данни не се изтриха!", what);
}

void xfer_accept(TRequest *request)
{
	if (request->xfer)
	{
		request->xfer->Accept(request);
		xfer_delete(request->xfer, "Получените");
		xfer_reject(request);
	}
}

#define document OFFICE_GROUP_CAST(TXferDocument)

static void xfer_transfer_type(mstr &m, TGroup *group) { m.cat(xfer_type_name(document->xferType)); }

static void xfer_connect_kind(mstr &m, TGroup *group)
{
	XferKind->Seek(document->key.xferKind);
	ConnectKind->Seek(XferKind->connectKind);
	m.cpy(ConnectKind->alias);
}

#undef document

#define court OFFICE_GROUP_CAST(TCourt)

static void xfer_title_court(mstr &m, TGroup *group)
{
	m.cat(court->name);
}

#undef court

static const offield xfer_list_titles[] =
{
	{ "F_COURT",	xfer_title_court },
	{ NULL, NULL }
};

static const offield xfer_list_fields[] =
{
	{ "F_TRANSFER_TYPE",	xfer_transfer_type },
	{ "F_CONNECT_KIND",	xfer_connect_kind },
	{ "F_NO_YEAR",		xfer_no_year },
	{ "F_COURT",		xfer_court },
	{ "F_OUTREG_NO",		NULL },
	{ "F_OUTREG_YEAR",	NULL },
	{ NULL, NULL }
};

static void xfer_print_list(TWindow *parent, long court, TGroupArray *array)
{
	TWaitWindow wait(parent, WAIT);
	TOffice office;

	if (office.Open("XferList.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");

		Court->Seek(court);
		office.Emit(NULL, tr1start, xfer_list_titles, Court);

		for (int i = 0; i < array->Count(); i++)
			office.Emit(tr1start, tr1final, xfer_list_fields, (*array)[i]);

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(&wait);
	}
}

#include "XferDeleteListWindow.h"
#include "XferDeleteListWindow.cpp"

#include "XferDeleteListDialog.h"
#include "XferDeleteListDialog.cpp"

void xfer_delete(TWindow *parent)
{
	TXferDeleteListDialog(parent, XFER_COURT_TYPES).Execute();
}

static void xfer_receive(TOutReg *outReg, TXferDocument *document, TReceiveInstance *rinst, TElectricTrouble *trouble)
{
	TXferDecision decision;

	decision.key = document->key;
	if (decision.Get())
	{
		XferKind->Seek(document->key.xferKind);
		ConnectKind->Seek(XferKind->connectKind);

		// 2011:356
		if (outReg->receiverType == UCN_INSTANCE_HIGHER)
		{
			rinst->lawsuitKind->SetC(XferKind->connectKind);
			if (rinst->xferKind)
				rinst->xferKind->SetL(document->key.xferKind);
			rinst->lawsuitNo->SetL(document->key.no);
			rinst->lawsuitYear->SetL(document->key.year);
			rinst->decisionKind->SetC(xferKind2Kind(decision.decisionKind));
			rinst->decisionDate->SetDate(decision.decisionDate);

			char court[SIZE_OF_UCN];
			itoa(document->key.courtSender, court, 10);
			rinst->court->SetS(court);
		}
		else
		{
			mstr m;

			m.printf("%s № %ld/%Dг. по %s %ld/%ldг. на %s", XferKind->Seek(decision.decisionKind), decision.decisionNo,
				&(CDate &) decision.decisionDate, ConnectKind->name, document->key.no, document->key.year,
				Court->Seek(document->key.courtSender));
			rinst->remark->SetS(str(m));
		}
	}

	strcpy(outReg->vssIndex, document->returnResult);
	load_names(trouble, decision.decisionFile);
	xfer_reject(outReg);
	outReg->xfer = document;
}

bool xfer_receive(TWindow *parent, TOutReg *outReg, TReceiveInstance *rinst, TElectricTrouble *trouble)
{
	TSender sender;

	if (get_receiver(outReg, &sender) && Court->Seek(sender.court))
	{
		if (!strchr(XFER_COURT_TYPES, Court->courtType))
			return false;

		msql m("T_XFER_DOCUMENT");
		TGArray<TXferDocument> documents;
		TStringArray strings;

		m.AddLong("F_OUTREG_NO", outReg->key.no);
		m.AddLong("F_OUTREG_YEAR", outReg->key.year);
		m.AddLong("F_COURT_SENDER", sender.court);
		m.AddLong("F_IO", XFER_RECEIVE);
		m.AddChar("F_XFER_TYPE", XFER_TYPE_RECEIVED);

		TQuery q(str(m));
		while (q.Read())
		{
			TXferDocument *document = new TXferDocument;

			*document << q;
			documents.Add(document);
			m.clear();
			document->Print(m);
			strings.Add(m.newstr());
		}

		int index = -1;

		if (documents.Count() == 0)
			error("Няма приети документи за връщането на делото/документа.");
		else if (documents.Count() == 1)
			index = 0;
		else
			index = select_string(parent, &strings, NULL, NULL, IDD_XFER_RECEIVE);

		if (index >= 0)
		{
			xfer_receive(outReg, documents.Detach(index), rinst, trouble);
			return true;
		}
	}
	else
		error("С изходящия документ не е изпратено дело, или получателя няма код на съда.");

	return false;
}

void xfer_reject(TOutReg *outReg)
{
	delete outReg->xfer;
	outReg->xfer = NULL;
}

void xfer_accept(TOutReg *outReg)
{
	if (outReg->xfer)
	{
		xfer_delete(outReg->xfer, "Получените");
		xfer_reject(outReg);
	}
}

// ----- TXferWebData --------------------------------------------------------
enum { SIZE_OF_XFER_JUDGE_TEXT = 250 + 1 };

class TXferWebData : public TXferTable
{
public:
	TXferWebData();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void Print(mstr &m);

	char judgeText[SIZE_OF_XFER_JUDGE_TEXT];
	TXferConnectData lower;
	long appealedKind;
	long appealedNo;
	CXferDate appealedDate;
	TXferConnectData jurisd;
	TXferConnectData reborn;

	long decisionKind;
	long decisionNo;
	CXferDate decisionDate;
	char decisionFile[SIZE_OF_XFER_PATH];
	char decisionId[SIZE_OF_UUID];
	CXferDate motivesDate;
	char motivesFile[SIZE_OF_XFER_PATH];
	CXferDate forceDate;
	char returnResult[SIZE_OF_VSS_INDEX];

	TXferKey outreg;
	long outregCourt;		// == courtReceiver
	CXferDate outregDate;

	CXferDate sessionDate;
	char decisionChar;
	long io;
	CXferDate xferDate;	// == Today
	char type;			// 2011:278 +related
};

TXferWebData::TXferWebData()
	: TXferTable("T_XFER_WEB", TXferWebData::Clone)
{
	Add(
		new TString("F_JUDGE_TEXT", judgeText, sizeof judgeText),
		NEW_XFER_CONNECT("LOWER", &lower),
		new TLong("F_APPEALED_KIND", &appealedKind),
		new TLong("F_APPEALED_NO", &appealedNo),
		new TXferDate("F_APPEALED_DATE", &appealedDate),
		NEW_XFER_CONNECT("JURISD", &jurisd),
		NEW_XFER_CONNECT("REBORN", &reborn),
		new TLong("F_DECISION_KIND", &decisionKind),
		new TLong("F_DECISION_NO", &decisionNo),
		new TXferDate("F_DECISION_DATE", &decisionDate),
		new TString("F_DECISION_FILE", decisionFile, sizeof decisionFile),
		new TString("F_DECISION_ID", decisionId, sizeof decisionId),
		new TXferDate("F_MOTIVES_DATE", &motivesDate),
		new TString("F_MOTIVES_FILE", motivesFile, sizeof motivesFile),
		new TXferDate("F_FORCE_DATE", &forceDate),
		new TString("F_RETURN_RESULT", returnResult, sizeof returnResult),
		NEW_XFERKEY("F_OUTREG_", &outreg),
		new TLong("F_OUTREG_COURT", &outregCourt),
		new TXferDate("F_OUTREG_DATE", &outregDate),
		new TXferDate("F_SESSION_DATE", &sessionDate),
		new TCharAlias("F_DECISION_CHAR", &decisionChar),
		new TLong("F_IO", &io),
		new TXferDate("F_XFER_DATE", &xferDate),
		new TCharAlias("F_TYPE", &type),
		NULL
	);

	descs = Descs + TABLE_XFER_WEB;
	xferDate = Today;
}

TGroup *TXferWebData::Clone()
{
	return new TXferWebData;
}

void TXferWebData::ExactCriteria(msql &m, const char *fields)
{
	TXferTable::ExactCriteria(m, fields);
	m.Add("F_SESSION_DATE");
	m.printf("= '%D'", &(CDate &) sessionDate);
	m.Add(this, "F_DECISION_CHAR");
}

void TXferWebData::Print(mstr &m)
{
	xferKind2Kind(key.xferKind);
	m.printf("%-15s %5ld/%5ld ", Kind->alias, key.no, key.year);
	xferKind2Kind(decisionKind);
	m.printf("%-15s ", Kind->alias);

	if (decisionNo)
		m.printf("%7ld ", decisionNo);
	else
		m.cat("        ");

	m.printf("%-D", &decisionDate);
}

// ----- TXferWeb ------------------------------------------------------------
class TXferWeb : public TXferWebData
{
public:
	TXferWeb(const TDecision *tDecision, const TSession *tSession, const TLawsuit *tLawsuit, const TOutReg *tOutReg);

	bool Delete();
	void LogAny(bool result, const char *what);
	void Send();

protected:
	const TDecision *decision;
	const TSession *session;
	const TLawsuit *lawsuit;
	const TOutReg *outReg;

	virtual bool Delete(bool exact);
	bool Init(TXferConnectData *connect, char connectType);
	bool Send(bool motiving);
};

TXferWeb::TXferWeb(const TDecision *tDecision, const TSession *tSession, const TLawsuit *tLawsuit, const TOutReg *tOutReg)
	: decision(tDecision), session(tSession), lawsuit(tLawsuit), outReg(tOutReg)
{
	key.xferKind = Kind->Xfer(lawsuit->kind);
	key.no = flag_key_no(&lawsuit->key);
	key.year = lawsuit->key.year;
	key.courtSender = COURT_CODE;

	decisionKind = Kind->Xfer(decision->kind);
	decisionDate = decision->fromDate;
	decisionNo = (PRINT_JUDGEMENTS == 1 || PRINT_JUDGEMENTS == 2 && decisionDate >= NewWebDate) ? decision->eventNo : 0;

	sprintf(decisionId,
		"%06d%02d-%04d-%04d-%02d%02d-%02d%07d%03d",
		lawsuit->key.no, lawsuit->key.year % 100,
		Kind->Xfer(lawsuit->kind), tDecision->surrouendoc ? 500 + tDecision->surrouendoc->extraNo : decisionKind,
		decision->date.day, decision->date.month,
		decision->date.year % 100, decision->eventNo, COURT_CODE);

	sessionDate = decision->date;
	decisionChar = tDecision->surrouendoc ? xfer_safe_chars[tDecision->surrouendoc->extraNo] : decision->kind;
	io = XFER_INVALID;
	type = lawsuit->key.type;
}

bool TXferWeb::Delete(bool exact)
{
	return TXferTable::Delete(exact);
}

bool TXferWeb::Delete()
{
	TXferWeb xferWeb(decision, session, lawsuit, outReg);
	return !xferWeb.Try() || xferWeb.Delete(true);
}

bool TXferWeb::Init(TXferConnectData *connect, char connectType)
{
	TConnect tConnect;
	return connect->connectYear || !tConnect.TryBy(lawsuit, connectType) || connect->Init(tConnect);
}

void TXferWeb::LogAny(bool result, const char *what)
{
	mstr m(result ? "webp:подготвен за " : "webp:грешка при ");
	m.cat(what);
	log(LOG_TRANSFER, 0, decision, str(m));
}

bool TXferWeb::Send(bool motiving)
{
	if (!Delete())
	{
		LogAny(false, "изтриване");
		return false;
	}

	mstr m;

	if (PRINT_COMPOSITIONS)
		m.printf("%s състав", Composition->Seek(session->composition));

	const char *judges[3];
	session2Judges(session, judges);

	for (int i = 0; i < 3 && atob(judges[i]); i++)
	{
		m.sep(", ");
		print_user(m, judges[i]);
	}

	strzcpy(judgeText, str(m), sizeof judgeText);

	TConnect connect;

	if (connect.TryBy(lawsuit, CONNECT_TYPE_LOWER_INSTANCE))
	{
		if (!connect.decisionKind)
			LogAny(false, "изпращане: appealed kind");
		else if (connect.decisionDate.Empty())
			LogAny(false, "изпращане: appealed date");
		// 2011:164 LPR;LRQ: send executor without lower
		else if (strchr(UCN_EXECUTORS, connect.ucnType))
			LogAny(false, "изпращане: ucn executor");
		else
		{
			appealedKind = Kind->Xfer(connect.decisionKind);
			appealedDate = connect.decisionDate;

			if (!lower.Init(connect))
				return false;
		}
	}

	if (!Init(&jurisd, CONNECT_TYPE_JURISDICTION) || !Init(&reborn, CONNECT_TYPE_RETURNED_LAWSUIT) ||
		!Init(&reborn, CONNECT_TYPE_RENEWED_LAWSUIT) || !Init(&reborn, CONNECT_TYPE_RERETURNED_LAWSUIT))
	{
		return false;
	}

	if (motiving)
	{
		char tKind = decision->kind;

		motivesDate = decision->returned;
		((TDecision *) decision)->kind = KIND_MOTIVES;
		xfer_name(decision, motivesFile, TYPE_HTML);
		((TDecision *) decision)->kind = tKind;
	}

	forceDate = decision->forceDate;

	if (outReg)
	{
		outreg.xferKind = Kind->Xfer(outReg->kind);
		outreg.no = outReg->key.no;
		outreg.year = outReg->key.year;
		outregDate = outReg->date;
		strcpy(returnResult, outReg->vssIndex);
		if ((outregCourt = sender2Court(outReg->receiver, outReg->receiverType)) == 0)
			return false;
	}

	io = XFER_SEND;
	return Insert();
}

void TXferWeb::Send()
{
	bool motiving = decision->flags & DECISION_MOTIVES;

	// 2009:278 LRQ: always send
	//if (!motiving || !decision->returned.Empty())
	{
		xfer_name(decision, decisionFile, TYPE_HTML);

		if (any(decisionFile))
			LogAny(Send(motiving), "изпращане");
		else
			log(LOG_TRANSFER, 0, decision, "изпращане: няма обезличен файл");
	}
}

// ----- functions -----------------------------------------------------------
void xfer_web(const TDecision *decision, const TSession *session, const TLawsuit *lawsuit)
{
	if (XFER_ACTIVE & XFER_WEBACTS)
	{
		msql m;
		TOutReg outReg;
		bool tOutReg = false;

		m.Begin(outReg.Name);
		m.AddLong("F_LAWSUIT_NO", lawsuit->key.no);
		m.AddLong("F_LAWSUIT_YEAR", lawsuit->key.year);
		m.AddChar("F_LAWSUIT_TYPE", lawsuit->key.type);
		m.AddChar("F_RECEIVER_TYPE", UCN_INSTANCE_HIGHER);
		m.AddOrder("F_DATE DESC");

		TQuery q(str(m));
		while (q.Read())
		{
			outReg << q;
			m.Begin("T_INREG_EVENT E JOIN T_INREG I ON I.F_NO = E.F_NO AND I.F_YEAR = E.F_YEAR", "I.F_NO");
			m.AddLong("E.F_OUT_NO", outReg.key.no);
			m.AddLong("E.F_OUT_YEAR", outReg.key.year);
			m.AddChar("I.F_DECISION_KIND", decision->kind);
			m.AddDate("I.F_DECISION_DATE", decision->date);

			TQuery r(str(m));
			if (r.Read())
			{
				tOutReg = true;
				break;
			}
		}

		TXferWeb(decision, session, lawsuit, tOutReg ? &outReg : NULL).Send();
	}
}

void xfer_web(const TOutReg *outReg, bool tDelete)
{
	if ((XFER_ACTIVE & XFER_WEBACTS) && strchr(TYPE_LAWSUITS, outReg->lawsuit.type))
	{
		TInReg inReg;

		inReg.key.no = outReg->inNo;
		inReg.key.year = outReg->inYear;

		if (inReg.Get("F_DECISION_KIND, F_DECISION_DATE, F_SESSION_KIND") && xfer_sendable(&inReg))
		{
			TLawsuit lawsuit;
			TDecision decision;
			TSession session;

			decision.key = session.key = lawsuit.key = outReg->lawsuit;
			decision.date = session.date = inReg.decisionDate;
			decision.kind = inReg.decisionKind;
			session.kind = inReg.sessionKind;

			if (decision.Try() && (decision.flags & DECISION_PUBLIC) && lawsuit.Get("F_KIND") && session.Get())
				TXferWeb(&decision, &session, &lawsuit, tDelete ? NULL : outReg).Send();
		}
	}
}

void xfer_web_delete(const TDecision *decision, const TLawsuit *lawsuit)
{
	if (XFER_ACTIVE & XFER_WEBACTS)
	{
		TXferWeb xferWeb(decision, NULL, lawsuit, NULL);
		xferWeb.io = XFER_WEB_DELETE;
		xferWeb.LogAny(xferWeb.Delete() && xferWeb.Insert(), "WEB изтриване");
	}
}

class TXferWebGroup : public TFloatGroup
{
public:
	TXferWebGroup();

	virtual void FormCriteria(msql &m);

	char lawKinds[SIZE_OF_KINDS];
	char decKinds[SIZE_OF_KINDS];
	CXferDate minXferDate;
	CXferDate maxXferDate;
};

TXferWebGroup::TXferWebGroup()
	: TFloatGroup("T_XFER_WEB")
{
	Add(
		new TCharList("F_LAW_KINDS", lawKinds, sizeof lawKinds),
		new TCharList("F_DEC_KINDS", decKinds, sizeof decKinds),
		new TXferDate("F_MIN_XFER_DATE", &minXferDate),
		new TXferDate("F_MAX_XFER_DATE", &maxXferDate),
		NULL
	);
}

void TXferWebGroup::FormCriteria(msql &m)
{
	BeginCriteria(m);

	if (!strequal(lawKinds, KIND_LAWSUITS))
		m.AddXferKinds("F_XFER_KIND", lawKinds);

	m.AddXferKinds("F_DECISION_KIND", decKinds);
	m.AddRange("F_XFER_DATE", minXferDate, maxXferDate);
	m.AddIsNull("F_IO", true);
	m.AddOrder("F_XFER_DATE, F_XFER_KIND, F_NO, F_YEAR, F_DECISION_KIND");
}

// ----- TXferWebPrint -------------------------------------------------------
class TXferWebPrint : public TXferWebData
{
public:
	TXferWebPrint() : read(false) { }

	sign Limited();

protected:
	bool read;
	sign limited;
};

sign TXferWebPrint::Limited()
{
	if (!read)
	{
		TLawsuit lawsuit;

		lawsuit.key.no = key.no;
		lawsuit.key.year = key.year;
		lawsuit.key.type = type ? type : kind2type(xferKind2Kind(key.xferKind));
		limited = lawsuit.Get("F_FLAGS") ? (lawsuit.flags & LAWSUIT_LIMITED) != 0 : -1;
	}

	return limited;
}

#define xferWeb OFFICE_GROUP_CAST(TXferWebPrint)

static void xferweb_decision_kind(mstr &m, TGroup *group)
{
	xferKind2Kind(xferWeb->decisionKind);
	m.cpy(Kind->name);
}

static void xferweb_decision_alias(mstr &m, TGroup *group)
{
	xferKind2Kind(xferWeb->decisionKind);
	m.cpy(Kind->alias);
}

static void xferweb_limited_short(mstr &m, TGroup *group)
{
	if (xferWeb->Limited())
		m.cat(xferWeb->Limited() == -1 ? "??" : "ДА");
}

#undef xferWeb

#include "printweb.cpp"

#include "XferWebListWindow.h"
#include "XferWebListWindow.cpp"

#include "XferWebListDialog.h"
#include "XferWebListDialog.cpp"

void xfer_web_query(TWindow *parent)
{
	TXferWebGroup group;
	TXferWebListDialog(parent, &group).Execute();
}

// ----- TExpertCaseNumber ---------------------------------------------------
enum { SIZE_OF_CASE_NUMBER = 21 + 1 };

class TXferCC : public TGroup
{
public:
	TXferCC(const char *name, TRCDKeyContainer *tContainer, char tLawKind);

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s) { TGroup::FloatCriteria(s); }

	virtual bool Insert();

	char caseNumber[SIZE_OF_CASE_NUMBER];
	long investId;
	char xferType;
	long io;

protected:
	TRCDKeyContainer *container;
	void CCNum();

private:
	char lawKind;
};

TXferCC::TXferCC(const char *name, TRCDKeyContainer *tContainer, char tLawKind)
	: TGroup(name, NULL), container(tContainer), lawKind(tLawKind)
{
	Add(
		new TString("F_CASE_NUMBER", caseNumber, sizeof caseNumber),
		new TLong("F_INVEST_ID", &investId),
		new TChar("F_XFER_TYPE", &xferType),
		new TLong("F_IO", &io),
		NULL
	);

	descs = Descs + TABLE_XFER_INVEST;
}

void TXferCC::CCNum()
{
	sprintf(caseNumber, "%03ld-%07ld-%ld-%04ld", COURT_CODE, flag_key_no(&container->key), container->key.year,
		Kind->Xfer(lawKind));
}

void TXferCC::ExactCriteria(msql &m, const char *fields)
{
	CCNum();

	m.Begin(Name, fields);
	m.Add(this, "F_CASE_NUMBER, F_INVEST_ID, F_XFER_TYPE, F_IO");
}

void TXferCC::FloatCriteria(msql &m, const char *fields)
{
	CCNum();

	m.Begin(Name, fields);
	m.Add(this, "F_CASE_NUMBER, F_XFER_TYPE, F_IO");
}

bool TXferCC::Insert()
{
	CCNum();
	return TGroup::Insert();
}

// ----- TXferInvest ---------------------------------------------------------
enum { SIZE_OF_RULING_ID = 9 + 1 };

class TXferInvest : public TXferCC
{
public:
	TXferInvest(TRCDKeyContainer *container, char lawKind);

	void Launch();

	char rulingId[SIZE_OF_RULING_ID];
	CXferDate rulingDate;
	mstr rulingText;
};

TXferInvest::TXferInvest(TRCDKeyContainer *container, char lawKind)
	: TXferCC("T_XFER_INVEST", container, lawKind)
{
	Add(
		new TString("F_RULING_ID", rulingId, sizeof rulingId),
		new TXferDate("F_RULING_DATE", &rulingDate),
		new TText("F_RULING_TEXT", &rulingText, SIZE_OF_SESSION_TEXT),
		NULL
	);
}

void TXferInvest::Launch()
{
	mstr m(XFER_CCEXP);

	CCNum();
	m.cat(caseNumber);
	message(str(m));
	electric_action(str(m), TYPE_HTML, ACTION_OPEN);
}

// ----- TXferExpert ---------------------------------------------------------
class TXferExpert : public TXferCC
{
public:
	TXferExpert(TRCDKeyContainer *container, char lawKind);

	virtual bool Delete(bool exact);
	virtual void ExactCriteria(msql &m, const char *fields = NULL);

	bool Delete(TGroup &group);
	bool Load(TCitizen &citizen);
	bool Load(TCitizen &citizen, TSideWind &sideWind);
	bool Log(const char *name, const char *oper);
	bool Save(TGroup &group, bool edit);

	long invExpId;
	long expertId;
	TXferCitizen expert;
	long expertUCLP;
	CXferDate notifyDate;
	CXferDate resignDate;

protected:
	char *const ucn;
};

TXferExpert::TXferExpert(TRCDKeyContainer *container, char lawKind)
	: TXferCC("T_XFER_EXPERT", container, lawKind), ucn(expert.egn)
{
	Add(
		new TLong("F_INVEXP_ID", &invExpId),
		new TLong("F_EXPERT_ID", &expertId),
		NEW_XFER_CITIZEN("F_EXPERT_", &expert),
		new TString("F_EXPERT_FAMILY", expert.family1, sizeof expert.family1),
		new TLong("F_EXPERT_UCLP", &expertUCLP),
		new TXferDate("F_NOTIFY_DATE", &notifyDate),
		new TXferDate("F_CHALLENGE_DATE", &resignDate),
		NULL
	);

	descs = Descs + TABLE_XFER_EXPERT;
}

bool TXferExpert::Delete(bool exact)
{
	return TXferCC::Delete(exact);
}

bool TXferExpert::Delete(TGroup &group)
{
	return group.Delete(true) ? (Log(group.Name, "Изтриване"), true) : Log(group.Name, "Грешка/2");
}

void TXferExpert::ExactCriteria(msql &m, const char *fields)
{
	TXferCC::ExactCriteria(m, fields);
	m.Add(this, "F_INVEXP_ID");
}

bool TXferExpert::Load(TCitizen &citizen)
{
	TKnownPerson person;
	bool tPerson;

	if (atob(ucn) && !ucn_valid(UCN_CITIZEN_UCN, ucn))
	{
		Log(ucn, "невалидно ЕГН");
		*ucn = '\0';
	}

	strupr(expert.name);
	strupr(expert.rename);
	strupr(expert.family1);

	msql m(person.Name);
	m.AddLong("F_EXPERT_ID", expertId);
	m.AddChar("F_INVOLVEMENT", INVOLVEMENT_EXPERT);

	TQuery q(str(m));
	if (q.Read())
	{
		person << q;
		tPerson = true;
	}
	else if (atob(ucn))
	{
		strcpy(person.ucn, ucn);
		person.ucnType = UCN_CITIZEN_UCN;
		person.involvement = INVOLVEMENT_EXPERT;
		tPerson = person.Try();
	}
	else
		tPerson = false;

	if (tPerson)
	{
		strcpy(citizen.ucn, person.ucn);
		citizen.ucnType = person.ucnType;
	}
	else if (atob(ucn))
	{
		strcpy(citizen.ucn, ucn);
		citizen.ucnType = UCN_CITIZEN_UCN;
	}
	else
		citizen.ucnType = UCN_CITIZEN_CODE;

	bool tCitizen = atob(citizen.ucn) && citizen.Try();

	if (
		!tCitizen || strcmp(citizen.name, expert.name) ||
		(any(expert.rename) && strcmp(citizen.reName, expert.rename)) ||
		strcmp(citizen.family, expert.family1)
	)
	{
		strcpy(citizen.name, expert.name);
		strcpy(citizen.reName, expert.rename);
		strcpy(citizen.family, expert.family1);
		if (!Save(citizen, tCitizen))
			return false;
	}

	if (expertUCLP)
	{
		TAddress address;

		UCLP->uclp = expertUCLP;
		strcpy(address.ucn, citizen.ucn);
		address.ucnType = citizen.ucnType;
		if (UCLP->Try() && !address.Try())
		{
			address.uclp = expertUCLP;
			address.Insert();
		}
	}

	if (!tPerson)
	{
		strcpy(person.ucn, citizen.ucn);
		person.ucnType = citizen.ucnType;
		person.expertId = expertId;
		person.involvement = INVOLVEMENT_EXPERT;
		if (!Save(person, false))
			return false;
	}
	else if (person.expertId != expertId)
	{
		person.expertId = expertId;
		if (!Save(person, true))
			return false;
	}

	TSideWind sideWind;
	sideWind << container;

	return Load(citizen, sideWind);
}

bool TXferExpert::Load(TCitizen &citizen, TSideWind &sideWind)
{
	bool tSideWind = false;

	*container << sideWind;
	sideWind << citizen;

	for (const char *inv = INVOLVEMENT_EXPERTS; *inv; inv++)
	{
		sideWind.involvement = *inv;
		if (sideWind.Try())
		{
			tSideWind = true;
			break;
		}
	}

	if (!tSideWind || sideWind.investId != investId || sideWind.invExpId != invExpId)
	{
		sideWind.investId = investId;
		sideWind.invExpId = invExpId;
		if (!Save(sideWind, tSideWind))
			return false;
	}

	return true;
}

bool TXferExpert::Log(const char *name, const char *oper)
{
	mstr m;
	m.printf("IX: %ld, %s: %s", invExpId, name, oper);
	log(LOG_TRANSFER, 0, container, str(m));
	return false;
}

bool TXferExpert::Save(TGroup &group, bool edit)
{
	return group.Save(edit) ? (Log(group.Name, edit ? "Презапис" : "Запис"), true) : Log(group.Name, "Грешка/1");
}

// ----- functions -----------------------------------------------------------
void xfer_send(TSubpoena *subpoena, TRCDKeyContainer *container, TSideWind *sideWind, char lawKind)
{
	if (XFER_ACTIVE & XFER_EXPERTS)
	{
		TXferExpert expert(container, lawKind);
		expert.investId = sideWind->investId;
		expert.invExpId = sideWind->invExpId;
		expert.xferType = XFER_TYPE_NOTIFY;

		bool tExpert = expert.Try();
		expert.notifyDate = subpoena->subpoenaReceived;
		expert.Save(expert, tExpert);
	}
}

void xfer_request(TSession *session, char lawKind)
{
	if (XFER_ACTIVE & XFER_EXPERTS)
	{
		TSideWind sideWind;
		msql m;
		TXferInvest invest(session, lawKind);

		sideWind << session;
		sideWind.FloatCriteria(m, "DISTINCT(F_INVEST_ID)");

		TQuery q(str(m));
		while (q.Read())
		{
			sideWind << q;

			if (sideWind.investId)
			{
				invest.investId = sideWind.investId;
				invest.xferType = XFER_TYPE_RESIGN;

				if (!invest.Try())
					invest.Insert();
			}
		}
	}
}

void xfer_assign(TSession *session, char lawKind)
{
	msql m;
	TGArray<TSideWind> distinct;

	TSideWind sideWind;
	TXferInvest invest(session, lawKind);

	// 2009:245 LRQ: fix for passive CC
	sideWind << session;
	sideWind.FloatCriteria(m, "DISTINCT(F_INVEST_ID)");

	TQuery p(str(m));
	while (p.Read())
	{
		sideWind << p;
		if (sideWind.investId)
		{
			TSideWind *tSideWind = new TSideWind;
			*tSideWind << sideWind;
			distinct.Add(tSideWind);
		}
	}

	TXferExpert expert(session, lawKind);

	expert.xferType = XFER_TYPE_ASSIGN;
	expert.io = XFER_RECEIVE;
	expert.FloatCriteria(m);
	m.AddOrder("F_INVEST_ID");

	TQuery q(str(m));
	while (q.Read())
	{
		TCitizen citizen;
		expert << q;

		if (expert.Load(citizen))
		{
			int i;

			for (i = 0; i < distinct.Count(); i++)
				if (distinct[i]->investId == expert.investId)
					break;

			if (i == distinct.Count())
			{
				invest.investId = expert.investId;
				invest.xferType = XFER_TYPE_ACCEPT;
				bool tInvest = invest.Try();

				sprintf(invest.rulingId, "%02d%02d%02d%03d",
					session->date.day, session->date.month,
					session->date.year % 100, session->kind);

				invest.rulingDate = session->date;
				invest.rulingText.cpy(session->text);
				expert.Save(invest, tInvest);
			}

			sideWind.key = session->key;
			m.Begin(sideWind.Name, "DISTINCT F_KIND, F_DATE");
			m.Add(session, "F_NO, F_YEAR, F_TYPE");
			m.Add("");
			m.printf("(F_DATE > %ld OR (F_DATE = %ld AND NOT F_KIND = '%c'))", session->date.Value(),
				session->date.Value(), session->kind);

			TQuery r(str(m));
			while (r.Read())
			{
				sideWind << r;
				expert.Load(citizen, sideWind);
			}
		}

		expert.Delete(expert);
	}
}

void xfer_resign(TSession *session, char lawKind)
{
	// 2017:111 testver s[] -> msql; 2017:137 official
	msql w;
	TXferExpert expert(session, lawKind);
	TSideWind sideWind;

	expert.xferType = XFER_TYPE_RESIGN;
	expert.io = XFER_RECEIVE;
	expert.FloatCriteria(w);

	TQuery q(str(w));
	while (q.Read())
	{
		expert << q;

		if (expert.resignDate >= session->date)
		{
			msql m(sideWind.Name);

			m.AddLong("F_NO", session->key.no);
			m.AddLong("F_YEAR", session->key.year);
			m.AddChar("F_TYPE", session->key.type);
			m.Add("F_DATE");
			m.printf(">= '%D'", &session->date);
			m.AddLong("F_INVEST_ID", expert.investId);
			m.AddLong("F_INVEXP_ID", expert.invExpId);

			TQuery r(str(m));
			while (r.Read())
			{
				constant names[] = { "T_SUBPOENA", "T_LINK_WIND", "T_EXPENSE", NULL };

				sideWind << r;

				for (int i = 0; names[i]; i++)
				{
					TBaseWind baseWind(names[i], NULL);
					baseWind << sideWind;

					if (baseWind.Try())
					{
						TRCDKeyContainer *const container = new TRCDKeyContainer("T_EXPERT", NULL);
						TVirtPtr VP(container);

						*container << baseWind;
						TXferExpert tExpert(container, lawKind);
						tExpert << expert;

						if (!tExpert.Delete(baseWind))
							break;
					}
				}

				expert.Delete(sideWind);
			}
		}

		expert.Delete(expert);
	}
}

void xfer_launch(TSession *session, char lawKind)
{
	TXferInvest(session, lawKind).Launch();
}

// ----- TXferEMail ----------------------------------------------------------
class TXferEMail : public TGroup
{
public:
	TXferEMail();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s) { TGroup::FloatCriteria(s); }

	CDateTime xferDate;	// +time
	TRCDKey key;
	char ucn[SIZE_OF_UCN];
	char email[SIZE_OF_EMAIL];
	mstr text;
	char fileName[SIZE_OF_XFER_PATH];
};

TXferEMail::TXferEMail()
	: TGroup("T_XFER_EMAIL", TXferEMail::Clone)
{
	Add(
		new TTimeStamp("F_XFER_DATE", &xferDate),
		NEW_RCDKEY("F_", &key),
		new TString("F_UCN", ucn, sizeof ucn),
		new TString("F_EMAIL", email, sizeof email),
		new TText("F_TEXT", &text, SIZE_OF_XFER_EMAIL_TEXT),
		new TString("F_FILENAME", fileName, sizeof fileName),
		NULL
	);

	descs = Descs + TABLE_XFER_EMAIL;
}

TGroup *TXferEMail::Clone()
{
	return new TXferEMail;
}

void TXferEMail::ExactCriteria(msql &m, const char *fields)
{
	TXferEMail::FloatCriteria(m, fields);
	m.Add(this, "F_NO, F_YEAR, F_TYPE, F_UCN, F_EMAIL");
}

void TXferEMail::FloatCriteria(msql &m, const char *fields)
{
	mstr d;

	xferDate.Print(d);
	m.Begin(Name, fields);
	m.Add("F_XFER_DATE");
	m.printf("= '%s'", str(d));
}

// ----- functions -----------------------------------------------------------
static void xfer_write_email(TRCDKeyContainer *container, TXferEMail &mail)
{
	TXferEMail tMail;
	tMail << mail;

	if (tMail.Try())
	{
		tMail << mail;
		if (!tMail.Update())
			log(LOG_TRANSFER, 0, container, "mail:грешка при презапис");
	}
	else if (!mail.Insert())
		log(LOG_TRANSFER, 0, container, "mail:грешка при запис");
}

void xfer_email(TRCDKeyContainer *container, char gop)
{
	if (XFER_EMAIL)
	{
		TInformed informed;
		TAddress address;
		TXferEMail mail;
		char text[SIZE_OF_XFER_EMAIL_TEXT];

		if (find_origin(container, mail.key) && strchr(TYPE_LAWSUITS, mail.key.type))
		{
			msql m("T_INFORMED", "F_UCN, F_EMAIL");

			m.AddLong("F_NO", mail.key.no);
			m.AddLong("F_YEAR", mail.key.year);
			m.AddChar("F_TYPE", mail.key.type);
			TQuery q(str(m));

			while (q.Read())
			{
				((CDate &) mail.xferDate) = Today;
				((CTime &) mail.xferDate) = Now();

				if (gop == LOG_DELETE)
					strcpy(text, "ИЗТРИВАНЕ ");
				else
				{
					*text = '\0';
					switch (XFER_EMAIL)
					{
						case XFER_EMAIL_TEXT : xfer_name(container, mail.fileName, TYPE_TEXT); break;
						case XFER_EMAIL_HTML : xfer_name(container, mail.fileName, TYPE_HTML); break;
						default : *mail.fileName = '\0';
					}
				}

				THTMLFile h;
				const char *tName = h.OpenEMail();

				// 2011:270 TRQ/FIX: full false -> depending on sessions and endocs
				query_event(h, container, !strchr(KIND_SESSIONS_ENDOCS, container->kind));
				// delete takes 10 + 1, so twice that should be ok
				h.ReadBack(text + strlen(text), 0, sizeof text - 21);
				h.Close();
				unlink(tName);

				informed << q;
				strcpy(mail.ucn, informed.ucn);
				mail.text.cpy(text);

				strcpy(address.ucn, informed.ucn);
				address.ucnType = UCN_INFORMED;

				if (address.Try("F_EMAIL") && any(address.email))
				{
					strcpy(mail.email, address.email);
					xfer_write_email(container, mail);
				}
				else
				{
					char s[SIZE_OF_LOG_TEXT];
					sprintf(s, "mail:%s: няма е-мейл адрес", informed.ucn);
					log(LOG_TRANSFER, 0, container, s);
				}

				if (any(informed.email))
				{
					strcpy(mail.email, informed.email);
					xfer_write_email(container, mail);
				}
			}
		}
		else
			log(LOG_TRANSFER, 0, container, "mail:няма origin");
	}
}

// ----- TXBankError ---------------------------------------------------------
class TXBankError
{
public:
	TXBankError() { }
	TXBankError(TRCDKeyContainer *container, const char *format, ...);
};

TXBankError::TXBankError(TRCDKeyContainer *container, const char *format, ...)
{
	va_list ap;
	mstr m("bank:");

	va_start(ap, format);
	m.vprintf(format, ap);
	va_end(ap);

	log(LOG_TRANSFER, 0, container, str(m));
	error("%s.", str(m));
}

// ----- TVirtualDecision ----------------------------------------------------
char TVirtualDecision::DecisionKind(char sessionResult)
{
	static const TVirtualDecision VirtualDecisions[] =
	{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
		{ RESULT_SUB_SETTLEMENT,	KIND_SETTLEMENT },
		{ RESULT_STOPPED,			KIND_SETTLEMENT },
		{ RESULT_WITH_ORDER,		KIND_ORDER },
#endif  // APPEAL || AREAL
#if COURT_TYPE == COURT_APPEAL
		{ RESULT_SETTLEMENT_205,	KIND_SETTLEMENT },
#endif  // APPEAL
#if COURT_TYPE == COURT_AREAL
		{ RESULT_WITH_DECREE,		KIND_DECREE },
#endif  // AREAL
		{ '\0', '\0' }
	};

	for (const TVirtualDecision *virtDec = VirtualDecisions; virtDec->sessionResult; virtDec++)
		if (virtDec->sessionResult == sessionResult)
			return virtDec->decisionKind;

	return NULL;
}

static bool xfer_partial_data(const TSession *session)
{
	return (session->flagsX & SESSION_X_CLOSEDD) || (session->forceDate.Empty() && (session->flags & SESSION_PRIVATE));
}

bool TVirtualDecision::AutoCreate(const TSession *session, TDecision *tDecision)
{
	char decisionKind = DecisionKind(session->result);

	if (decisionKind)
	{
		*tDecision << session;
		tDecision->kind = decisionKind;
		tDecision->sessionKind = session->kind;
		tDecision->fromDate = session->date;
		// 2011:334; 2012:005 FIX: text.clear() -> full set decision flags
		tDecision->flags = xfer_partial_data(session) ? 0 : DECISION_PUBLIC;
		// 2016:169 emulate PUBLTREG for bank
		tDecision->flagsX = session->flagsY & SESSION_Y_PUBLTREG ? DECISION_X_PUBLTREG : 0;
	#if TESTVER
		// 2018-03-12: LPR/IRQ: new unique id-s after date
		tDecision->virtualAct = session->date > XFER_BANK_DATE;
	#else  // TESTVER
		//tDecision->virtualAct = true;  // 2018-11-21: LPR/URQ: FIX?
	#endif  // TESTVER
		return true;
	}

	return false;
}

// ----- TXferBank -----------------------------------------------------------
class TXferBank : public TXMLFile
{
public:
	virtual void EmitMain() = 0;
	virtual TRCDKeyContainer *Main() = 0;

protected:
	TXferBank(TLawsuit *tLawsuit, const char *tSuffix = NULL);

	TLawsuit *lawsuit;
	const char *suffix;

	void EmitStart(XFER_ACTION action = XFER_IGNORE);
	void EmitId(const char *tag, TRCDKeyContainer *container);
	void EmitCTag(const char *prefix, const char *tag, const char *s);
	void EmitCitizen(TRCDKeyContainer *container, const char *prefix, char ucnType, const char *ucn);
	bool EmitSideWind(TRCDKeyContainer *container, TSideWind *sideWind);
	void EmitSideWinds(TRCDKeyContainer *container, TGroupArray *sideWinds);
	void EmitJudge(TRCDKeyContainer *container, const char *ucn);
	void EmitSurround(TSurround *surround, TGroupArray *sideWinds, XFER_ACTION action);
	void EmitSessionData(TSession *session, TGArray<TSurround> *surrounds, XFER_ACTION action);
	void EmitDecisionData(TDecision *session, XFER_ACTION action);
	void EmitInRegData(TInReg *inReg, TGroupArray *sideWinds, XFER_ACTION action);
	void EmitResult(const char *tag, char resolution, const char *remark);
	void EmitOutReg(TOutReg *outReg, TInReg *inReg, XFER_ACTION action);
	void EmitSessionAddFullChain(TSession *session, TGArray<TSurround> *surrounds, TGArray<TDecision> *decisions);
	void EmitDecisionAddFullChain(TDecision *decision);
	void EmitFinal();

	static void LoadSurrounds(TSession *session, TGArray<TSurround> *surrounds);
};

TXferBank::TXferBank(TLawsuit *tLawsuit, const char *tSuffix)
	: lawsuit(tLawsuit), suffix(tSuffix ? tSuffix : "events")
{
}

void TXferBank::EmitStart(XFER_ACTION action)
{
	mstr m;

	while (!OpenBank(m, suffix, Main()))
	{
		if (!is_sysadmin())
			m.cat(" Обърнете се към администратора на системата.");
		m.cat("\n\nЖелаете ли да опитате отново?");

		if (!ask("%s", str(m)))
		{	// 2017:033 LPR: don't report again
			log(LOG_TRANSFER, 0, Main(), "bank:отказан нов опит");
			throw new TXBankError();
		}

		m.clear();
		TConfigure::GetCurrent("transfer", "BANKRUPT_PATH", true);
	}

	OpenTag("Transfer");
	Emit("program", "CAC");
	PrintF("version", "%.4lf", atof(VERSION));
	OpenTag("Case");
	{
		EmitId("case_id", lawsuit);
		Emit("case_action", action);
		Emit("case_court", COURT_CODE);
		Emit("case_kind", Kind->Xfer(lawsuit->kind));
		Emit("case_no", lawsuit->key.no);
		Emit("case_year", lawsuit->key.year);
		Emit("case_date", lawsuit->date);
	}
}

void TXferBank::EmitId(const char *tag, TRCDKeyContainer *container)
{
	char id[SIZE_OF_UUID];
	xfer_uuid(id, container);
	Emit(tag, id);
}

void TXferBank::EmitCTag(const char *prefix, const char *tag, const char *s)
{
	mstr m;
	m.printf("%s_%s", prefix, tag);
	Emit(str(m), s);
}

void TXferBank::EmitCitizen(TRCDKeyContainer *container, const char *prefix, char ucnType, const char *ucn)
{
	TCitizen citizen;
	mstr tag;

	citizen.ucnType = ucnType;
	strcpy(citizen.ucn, ucn);

	if (!citizen.Get())
		throw TXBankError(container, "липсващо лице %c %s", ucnType, ucn);

	EmitCTag(prefix, "name_1", citizen.name);

	if (any(citizen.reName))
		EmitCTag(prefix, "rename", citizen.reName);

	EmitCTag(prefix, "family_1", citizen.family);

	if (any(citizen.reFamily))
		EmitCTag(prefix, "family_2", citizen.reFamily);

	if (is_real_ucn(&citizen))
		EmitCTag(prefix, "egn", citizen.ucn);
}

bool TXferBank::EmitSideWind(TRCDKeyContainer *container, TSideWind *sideWind)
{
	if (!Involvement->Xfer(sideWind->involvement) || ((container->key.type == TYPE_INREG ||
		container->key.type == TYPE_SURROUND) && !sideWind->Appealer()) || !strchr(UCN_REAL_SIDES, sideWind->ucnType))
	{
		return false;
	}

	OpenTag("Side");
	{
		char id[SIZE_OF_UUID];

		xfer_uuid(id, container, sideWind->involvement, sideWind->ucnType, sideWind->ucn);
		Emit("side_id", id);
		Emit("side_involvement", Involvement->xfer);
		if (strchr(UCN_CITIZENS, sideWind->ucnType))
		{
			OpenTag("side_citizen");
			EmitCitizen(container, "side", sideWind->ucnType, sideWind->ucn);
			CloseTag("side_citizen");
		}
		else	// UCN_FIRMS, pre-checked above
		{
			TFirm firm;

			firm.ucnType = sideWind->ucnType;
			strcpy(firm.ucn, sideWind->ucn);

			if (!firm.Get())
				throw TXBankError(container, "липсващо лице %c %s", sideWind->ucnType, sideWind->ucn);

			OpenTag("side_firm");
			{
				firm.name[XFER_FIRM_NAME] = '\0';
				Emit("side_name", firm.name);

				if (any(firm.bulstat))
					Emit("side_bulstat", firm.bulstat);
			}
			CloseTag("side_firm");
		}
	}
	CloseTag("Side");

	return true;
}

void TXferBank::EmitSideWinds(TRCDKeyContainer *container, TGroupArray *sideWinds)
{
	int count = 0;

	for (int i = 0; i < sideWinds->Count(); i++)
		if (EmitSideWind(container, (TSideWind *) (*sideWinds)[i]))
			count++;

	if (!count)
		throw TXBankError(container, "няма значещи страни");
}

void TXferBank::EmitJudge(TRCDKeyContainer *container, const char *ucn)
{
	OpenTag("Judge");
	{
		char id[SIZE_OF_UUID];

		xfer_uuid(id, container, INVOLVEMENT_JUDGE, UCN_CITIZEN_UCN, ucn);
		Emit("judge_id", id);
		EmitCitizen(container, "judge", UCN_CITIZEN_UCN, ucn);
	}
	CloseTag("Judge");
}

void TXferBank::EmitSurround(TSurround *surround, TGroupArray *sideWinds, XFER_ACTION action)
{
	TGArray<TSideWind> tSideWinds;

	if (!sideWinds)
	{
		load_sidewinds(surround, &tSideWinds);
		sideWinds = &tSideWinds;
	}

	OpenTag("Surround");
	{
		mstr m;

		EmitId("surround_id", surround);
		Emit("surround_action", action);
		Emit("surround_no", surround->key.no);
		Emit("surround_year", surround->key.year);
		Emit("surround_date", surround->date);
		if (strchr(KIND_TEXTUAL_SURROUNDS, surround->kind))
			m.cpy(surround->text);
		else
		{
			m.cpy(Kind->Seek(surround->kind));
			if (any(surround->text))
				m.printf(" %s", surround->text);
		}
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		Emit("surround_text", str(m));
#endif  // APPEAL || ADMIN || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL
		Emit("surround_text", surround->kind == KIND_BANKRUPT_SURROUND ? "Молба" : str(m));
#endif  // AREAL
		EmitSideWinds(surround, sideWinds);
	}
	CloseTag("Surround");
}

void TXferBank::EmitSessionData(TSession *session, TGArray<TSurround> *surrounds, XFER_ACTION action)
{
	EmitId("session_id", session);
	Emit("session_action", action);
	Emit("session_kind", Kind->Xfer(session->kind));
	Emit("session_date", session->date);
	Emit("session_time", session->time);
	Emit("session_result", Result->Xfer(session->result));
	// 2011:334 closedd check; 2011:343 function check
	Emit("session_text", (xfer_partial_data(session) || TVirtualDecision::DecisionKind(session)) ? "" : str(session->text));
	EmitJudge(session, session->judge);

	TGArray<TSurround> tSurrounds;

	// 2011:320 LPR: count check
	if (!surrounds || !surrounds->Count())
	{
		LoadSurrounds(session, &tSurrounds);
		surrounds = &tSurrounds;
	}

	for (int i = 0; i < surrounds->Count(); i++)
	{
		TSurround *surround = (*surrounds)[i];

		OpenTag("Surround");
		{
			EmitId("surround_id", surround);
			Emit("surround_no", surround->key.no);
			Emit("surround_year", surround->key.year);
		}
		CloseTag("Surround");
	}
}

void TXferBank::EmitDecisionData(TDecision *decision, XFER_ACTION action)
{
	if (decision->flagsX & DECISION_X_PUBLTREG)
	{
		OpenTag("extensions");
		{
			OpenTag("brra");
			{
				Emit("publish", 1);
			}
			CloseTag("brra");
		}
		CloseTag("extensions");
	}

	char extraNo = '\0';

	if (decision->surrouendoc)
		extraNo = (char) decision->surrouendoc->extraNo;
	else if (decision->virtualAct)
		extraNo = KIND_VIRTUAL_EXTRA_NO;

	if (extraNo)
	{
		char id[SIZE_OF_UUID];

		xfer_uuid(id, decision->key.no, decision->key.year, decision->key.type, decision->sessionKind, &decision->date,
			NULL, extraNo, '\0', NULL);
		Emit("act_id", id);
	}
	else
		EmitId("act_id", decision);

	Emit("act_action", action);
	Emit("act_kind", Kind->Xfer(decision->kind));
	if (PRINT_JUDGEMENTS && decision->eventNo)
		Emit("act_no", decision->eventNo);
	Emit("act_date", decision->fromDate);
	for (const char *accomply = decision->accomplys; *accomply; accomply++)
		if (Accomply->Xfer(*accomply))
			Emit("act_reason", Accomply->xfer);
	if (decision->debtorStatus && DebtorStatus->Xfer(decision->debtorStatus))
		Emit("act_debtor_status", DebtorStatus->xfer);
	Emit("act_text", decision->flags & DECISION_PUBLIC ? str(decision->text) : "");
	if (action != XFER_IGNORE && action != XFER_DELETE && (decision->flags & DECISION_PUBLIC))
	{
		// 2016:169 use and require PDF for TREG
		int type = SIGN_PDFS && (decision->flagsX & DECISION_X_PUBLTREG) ? TYPE_PORTREG : TYPE_IMAGE;
		char decisionKind = decision->kind;
		char name[SIZE_OF_PATH];

		if (strchr(RESULT_SESSIONS, decision->result))
			decision->kind = decision->sessionKind;

		xfer_name(decision, name, type);
		decision->kind = decisionKind;

		if (exist(name))
		{
			if (!EmitFile("act_image", name, Electrics[type].what))
				throw TXBankError(Main(), "грешка при четене на %s файл", Electrics[type].what);
		}
		else if (type == TYPE_PORTREG)
			throw TXBankError(Main(), "няма присъединен %s файл", PORTABLE_TYPE);
	}
}

void TXferBank::EmitInRegData(TInReg *inReg, TGroupArray *sideWinds, XFER_ACTION action)
{
	EmitId("appeal_id", inReg);
	Emit("appeal_action", action);
	Emit("appeal_kind", Kind->Xfer(inReg->kind));
	Emit("appeal_no", inReg->key.no);
	Emit("appeal_year", inReg->key.year);
	Emit("appeal_date", inReg->date);
	EmitSideWinds(inReg, sideWinds);
}

void TXferBank::EmitResult(const char *tag, char resolution, const char *remark)
{
	mstr result;

	if (resolution != RESOLUTION_FAKE)
		result.cat(Resolution->Seek(resolution));

	if (any(remark))
	{
		result.sep("\n");
		result.cat(remark);
	}

	if (any(result))
		Emit(tag, str(result));
}

void TXferBank::EmitOutReg(TOutReg *outReg, TInReg *inReg, XFER_ACTION action)
{
	OpenTag("SendTo");
	{
		char id[SIZE_OF_UUID];

		xfer_uuid(id, outReg, inReg);
		Emit("sendto_id", id);
		Emit("sendto_action", action);
		Emit("sendto_kind", Kind->Xfer(outReg->kind));
		Emit("sendto_no", outReg->key.no);
		Emit("sendto_year", outReg->key.year);
		Emit("sendto_date", outReg->date);

		TSender sender;

		if (!get_receiver(outReg, &sender))
			throw TXBankError(outReg, "липсващ адресант");
		if (!sender.court)
			throw TXBankError(outReg, "невалиден адресант");

		Emit("sendto_court", sender.court);

		if (!outReg->returned.Empty())
		{
			OpenTag("return");
			{
				TConnect vzConnect;
				TConnect ksConnect;

				vzConnect.key = ksConnect.key = outReg->key;
				vzConnect.connectType = CONNECT_TYPE_VZ_LAWSUIT;
				ksConnect.connectType = CONNECT_TYPE_KS_LAWSUIT;

				bool tVZConnect = vzConnect.Try();
				bool tKSConnect = ksConnect.Try();

				if (tVZConnect || tKSConnect)
				{
					OpenTag("extensions");

					if (tVZConnect)
					{
						Emit("return_result_no", vzConnect.connectNo);
						Emit("return_result_year", vzConnect.connectYear);
					}

					if (tKSConnect)
					{
						Emit("return_result2_no", ksConnect.connectNo);
						Emit("return_result2_year", ksConnect.connectYear);
					}

					CloseTag("extensions");
				}

				EmitResult("return_result", outReg->resultI, str(outReg->remarkVZ));
				EmitResult("return_result2", outReg->resultKS, str(outReg->remarkKS));
			}

			CloseTag("return");
		}
	}
	CloseTag("SendTo");
}

void TXferBank::EmitSessionAddFullChain(TSession *session, TGArray<TSurround> *surrounds, TGArray<TDecision> *decisions)
{
	OpenTag("Session");
	{
		EmitSessionData(session, surrounds, XFER_ADD);

		for (int i = 0; i < decisions->Count(); i++)
			EmitDecisionAddFullChain((*decisions)[i]);

		TDecision tDecision;

		if (TVirtualDecision::AutoCreate(session, &tDecision))
			EmitDecisionAddFullChain(&tDecision);
	}
	CloseTag("Session");
}

void TXferBank::EmitDecisionAddFullChain(TDecision *decision)
{
	OpenTag("Act");
	{
		EmitDecisionData(decision, XFER_ADD);

		TInReg inReg;
		TInRegEvent inRegEvent;
		TOutReg outReg;
		msql m(inReg.Name);

		m.AddLong("F_LAWSUIT_NO", decision->key.no);
		m.AddLong("F_LAWSUIT_YEAR", decision->key.year);
		m.AddChar("F_LAWSUIT_TYPE", decision->key.type);
		m.AddChar("F_DECISION_KIND", decision->kind);
		m.AddDate("F_DECISION_DATE", decision->date);

		TQuery q(str(m));
		while (q.Read())
		{
			inReg << q;
			TGArray<TSideWind> sideWinds;
			load_sidewinds(&inReg, &sideWinds);

			OpenTag("Appeal");
			{
				EmitInRegData(&inReg, &sideWinds, XFER_ADD);

				inRegEvent.key = inReg.key;
				m.Begin(inRegEvent.Name, "DISTINCT F_OUT_NO, F_OUT_YEAR");
				m.Add(&inRegEvent, "F_NO, F_YEAR, F_TYPE");
				m.AddIsNull("F_OUT_NO", false);

				TQuery p(str(m));
				while (p.Read())
				{
					inRegEvent << p;
					outReg.key.no = inRegEvent.outNo;
					outReg.key.year = inRegEvent.outYear;

					if (!outReg.Get())
						throw TXBankError(&inReg, "липсващ изходящ документ");

					EmitOutReg(&outReg, &inReg, XFER_ADD);
				}
				inRegEvent.outNo = 0;	// avoid OutRegCriteria()
			}
			CloseTag("Appeal");
		}
	}
	CloseTag("Act");
}

void TXferBank::EmitFinal()
{
	CloseTag("Case");
	CloseTag("Transfer");

	if (!Close())
		throw TXBankError(lawsuit, "грешка %d при запис във файл", errno);
}

void TXferBank::LoadSurrounds(TSession *session, TGArray<TSurround> *surrounds)
{
	TGArray<TSurroundment> surroundments;
	TSurround *surround = new TSurround;

	if (session->result != RESULT_FIXED)
		try2surround(session, &surroundments, "F_NO, F_YEAR, F_TYPE");

	for (int i = 0; i < surroundments.Count(); i++)
	{
		*surround << surroundments[i];

		if (surround->Try("F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_TEXT"))
		{
			surrounds->Add(surround);
			surround = new TSurround;
		}
		else
			throw TXBankError(session, "липсващ съпровождащ %d/%d", surround->key.no, surround->key.year);
	}

	delete surround;
}

bool xfer_bank(TXferBank &bank)
{
	try
	{
		bank.EmitMain();
		log(LOG_TRANSFER, 0, bank.Main(), "bank:подготвен за изпращане");
	}
	catch (TXBankError)
	{
		bank.Remove();
		return false;
	}

	return true;
}

// ----- TXferBankCase -------------------------------------------------------
class TXferBankCase : public TXferBank
{
public:
	TXferBankCase(TLawsuit *lawsuit, TGroupArray *tSideWinds, TRequest *tRequest, XFER_ACTION tAction);

	virtual void EmitMain();
	virtual TRCDKeyContainer *Main() { return lawsuit; }

protected:
	TGroupArray *sideWinds;
	TRequest *request;
	XFER_ACTION action;
};

TXferBankCase::TXferBankCase(TLawsuit *lawsuit, TGroupArray *tSideWinds, TRequest *tRequest, XFER_ACTION tAction)
	: TXferBank(lawsuit, "case"), sideWinds(tSideWinds), request(tRequest), action(tAction)
{
}

void TXferBankCase::EmitMain()
{
	TGArray<TSideWind> tSideWinds;

	EmitStart(action);

	OpenTag("InDoc");
	{
		TRequest tRequest;

		if (!request)
		{
			tRequest.key = lawsuit->source;
			tRequest.kind = lawsuit->sourceKind;

			if (tRequest.key.no)
			{
				if (!tRequest.Get("F_DATE"))
					throw TXBankError(Main(), "липсващ входящ документ");
			}
			else
				tRequest.date = lawsuit->date;

			request = &tRequest;
		}

		EmitId("indoc_id", request);
		Emit("indoc_kind", Kind->Xfer(request->kind));
		if (request->key.no)
			Emit("indoc_no", request->key.no);
		Emit("indoc_year", request->key.year);
		Emit("indoc_date", request->date);
	}
	CloseTag("InDoc");

	if (!sideWinds)
	{
		load_sidewinds(lawsuit, &tSideWinds);
		sideWinds = &tSideWinds;
	}

	EmitSideWinds(lawsuit, sideWinds);
	EmitJudge(lawsuit, lawsuit->judge);

	EmitFinal();

	if (action == XFER_ADD && !(lawsuit->flagsX & LAWSUIT_X_BANKRUPT))
	{
		lawsuit->flagsX |= LAWSUIT_X_BANKRUPT;
		lawsuit->Update("F_FLAGS_X");
	}
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
bool xfer_bank_case(TLawsuit *lawsuit, TGroupArray *sideWinds, TRequest *request, XFER_ACTION action)
{
	if ((XFER_ACTIVE & XFER_WEBANKS) && strchr(KIND_LAWSUITS, lawsuit->kind) &&
		strchr(KIND_INCONSISTENCE_REQUESTS, lawsuit->sourceKind))
	{
		if (!(lawsuit->flagsX & LAWSUIT_X_BANKRUPT))
		{
			if (action == XFER_DELETE)
				return false;

			action = XFER_ADD;
		}

		return xfer_bank(TXferBankCase(lawsuit, sideWinds, request, action));
	}

	return false;
}
#endif  // APPEAL || AREAL

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
bool xfer_bank_case(TLawsuit *, TGroupArray *, TRequest *, XFER_ACTION)
{
	return false;
}
#endif  // ADMIN || MILITARY || REGIONAL

// ----- TXferBankSurround ---------------------------------------------------
class TXferBankSurround : public TXferBank
{
public:
	TXferBankSurround(TLawsuit *lawsuit, TSurround *tSurround, TGroupArray *tSideWinds, XFER_ACTION tAction);

	virtual void EmitMain();
	virtual TRCDKeyContainer *Main() { return surround; }

protected:
	TSurround *surround;
	TGroupArray *sideWinds;
	XFER_ACTION action;
};

TXferBankSurround::TXferBankSurround(TLawsuit *lawsuit, TSurround *tSurround, TGroupArray *tSideWinds, XFER_ACTION tAction)
	: TXferBank(lawsuit), surround(tSurround), sideWinds(tSideWinds), action(tAction)
{
}

void TXferBankSurround::EmitMain()
{
	EmitStart();
	EmitSurround(surround, sideWinds, action);
	EmitFinal();
}

static bool xfer_bank_case_check(TLawsuit *lawsuit)
{
	return (lawsuit->flagsX & LAWSUIT_X_BANKRUPT) || xfer_bank_case(lawsuit, NULL, NULL, XFER_ADD);
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
static TLawsuit *xfer_bank_case_prime(TPrimeContainer *container, TLawsuit *tLawsuit)
{
	TRCDKey &key = container->lawsuit;

	if ((XFER_ACTIVE & XFER_WEBANKS) && key.type && strchr(TYPE_TRADEALIKE_LAWS, key.type))
	{
		if (_primary_group)
			*tLawsuit << _primary_group;

		if (tLawsuit->key != key)
		{
			tLawsuit->key = key;

			if (!tLawsuit->Get())
			{
				TXBankError(container, "липсващо дело");
				return NULL;
			}
		}

		return xfer_bank_case_check(tLawsuit) ? tLawsuit : NULL;
	}

	return NULL;
}
#endif  // APPEAL || AREAL

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
static TLawsuit *xfer_bank_case_prime(TPrimeContainer *, TLawsuit *)
{
	return NULL;
}
#endif  // ADMIN || MILITARY || REGIONAL

static TLawsuit *xfer_bank_case_ioreg(TIOReg *ioReg, TLawsuit *lawsuit)
{
	return Kind->Xfer(ioReg->kind) ? xfer_bank_case_prime(ioReg, lawsuit) : NULL;
}

void xfer_bank_surround(TLawsuit *lawsuit, TSurround *surround, TGroupArray *sideWinds, XFER_ACTION action)
{
	TLawsuit tLawsuit;

	if (lawsuit ? xfer_bank_case_check(lawsuit) : (lawsuit = xfer_bank_case_prime(surround, &tLawsuit)) != NULL)
		xfer_bank(TXferBankSurround(lawsuit, surround, sideWinds, action));
}

// ----- TXferBankEvent ------------------------------------------------------
class TXferBankEvent : public TXferBank
{
public:
	TXferBankEvent(TLawsuit *lawsuit, TSession *tSession, TGArray<TSurround> *tSurrounds, TDecision *tDecision,
		TInReg *tInReg, TGroupArray *tSideWinds, TOutReg *tOutReg, XFER_ACTION tAction);

	virtual void EmitMain();
	virtual TRCDKeyContainer *Main();

protected:
	TSession *session;
	TGArray<TSurround> *surrounds;
	TDecision *decision;
	TInReg *inReg;
	TGroupArray *sideWinds;
	TOutReg *outReg;
	XFER_ACTION action;
};

TXferBankEvent::TXferBankEvent(TLawsuit *lawsuit, TSession *tSession, TGArray<TSurround> *tSurrounds, TDecision *tDecision,
	TInReg *tInReg, TGroupArray *tSideWinds, TOutReg *tOutReg, XFER_ACTION tAction)
	: TXferBank(lawsuit), session(tSession), decision(tDecision), surrounds(tSurrounds), inReg(tInReg),
		sideWinds(tSideWinds), outReg(tOutReg), action(tAction)
{
}

void TXferBankEvent::EmitMain()
{
	EmitStart();

	OpenTag("Session");
	{
		EmitSessionData(session, surrounds, decision ? XFER_IGNORE : action);

		if (decision)
		{
			OpenTag("Act");
			{
				EmitDecisionData(decision, inReg ? XFER_IGNORE : action);

				if (outReg)
				{
					msql m;
					TInRegEvent inRegEvent;
					TInReg tInReg;

					inRegEvent.OutCriteria(outReg);
					inRegEvent.FloatCriteria(m, "F_NO, F_YEAR, F_TYPE");

					TQuery q(str(m));
					while (q.Read())
					{
						inRegEvent << q;
						tInReg.key = inRegEvent.key;

						if (!tInReg.Get())
						{
							TXBankError(&inRegEvent, "липсващ входящ документ");
							return;
						}

						TGArray<TSideWind> sideWinds;
						load_sidewinds(&tInReg, &sideWinds);

						OpenTag("Appeal");
						{
							EmitInRegData(&tInReg, &sideWinds, XFER_IGNORE);
							EmitOutReg(outReg, &tInReg, action);
						}
						CloseTag("Appeal");
					}

				}
				else if (inReg)
				{
					OpenTag("Appeal");
					EmitInRegData(inReg, sideWinds, action);
					CloseTag("Appeal");
				}
			}
			CloseTag("Act");
		}
		else if (action != XFER_DELETE)
		{
			TDecision tDecision;

			if (TVirtualDecision::AutoCreate(session, &tDecision))
			{
				OpenTag("Act");
				EmitDecisionData(&tDecision, action);
				CloseTag("Act");
			}
		}
	}
	CloseTag("Session");

	EmitFinal();
}

TRCDKeyContainer *TXferBankEvent::Main()
{
	return outReg ? (TRCDKeyContainer *) outReg : inReg ? (TRCDKeyContainer *) inReg :
		decision ? (TRCDKeyContainer *) decision : (TRCDKeyContainer *) session;
}

void xfer_bank_event(TLawsuit *lawsuit, TSession *session, TGArray<TSurround> *surrounds, TDecision *decision, TInReg *inReg,
	TGroupArray *sideWinds, TOutReg *outReg, XFER_ACTION action)
{
	TLawsuit tLawsuit;
	TSession tSession;
	TGArray<TSurround> tSurrounds;
	TDecision tDecision;
	TInReg tInReg;
	TGArray<TSideWind> tSideWinds;

	if (!(XFER_ACTIVE & XFER_WEBANKS))
		return;

	if (lawsuit && !xfer_bank_case_check(lawsuit))
		return;

	if (session && !Result->Xfer(session->result))
		return;

	if (decision && !Kind->Xfer(decision->kind))
		return;

	if (outReg)
	{
		if (!lawsuit && (lawsuit = xfer_bank_case_ioreg(outReg, &tLawsuit)) == NULL)
			return;

		if (!inReg)
		{
			tInReg.key.no = outReg->inNo;
			tInReg.key.year = outReg->inYear;

			if (!tInReg.Get())
			{
				TXBankError(outReg, "липсващ входящ документ");
				return;
			}

			inReg = &tInReg;
		}
	}

	if (inReg)
	{
		if (!lawsuit && (lawsuit = xfer_bank_case_ioreg(inReg, &tLawsuit)) == NULL)
			return;

		// 2012:152 FIX: reject non-decision
		if (!inReg->decisionKind || !strchr(KIND_ENDOCS, inReg->decisionKind))
			return;

		if (!decision)
		{
			tDecision.key = inReg->lawsuit;
			tDecision.kind = inReg->decisionKind;
			tDecision.date = inReg->decisionDate;

			if (inReg->extraNo)
			{
				TSurroundment *const surrouendoc = (TSurroundment *) inReg->AppealedAct(false, false, "*");

				if (!surrouendoc)
				{
					TXBankError(inReg, "липсващ акт по възражение");
					return;
				}

				TVirtPtr VP(surrouendoc);
				tDecision.Init(surrouendoc);
			}
			else if (!tDecision.kind || !tDecision.Try())
			{
				tSession << tDecision;
				tSession.kind = inReg->sessionKind;

				if (!tSession.kind || !tSession.Get())
				{
					TXBankError(inReg, "липсващо заседание или съдебен акт");
					return;
				}

				if (!Result->Xfer(tSession.result))
					return;

				if (!TVirtualDecision::AutoCreate(&tSession, &tDecision))
				{
					// 2011:293 LRQ: don't send anything, not an error
					//TXBankError(&tSession, "заседанието няма съдебен акт");
					return;
				}

				session = &tSession;
			}

			decision = &tDecision;

			if (!Kind->Xfer(decision->kind))
				return;
		}

		if (!sideWinds)
		{
			load_sidewinds(inReg, &tSideWinds);
			sideWinds = &tSideWinds;
		}
	}

	if (decision)
	{
		if (!session)
		{
			tSession.key = decision->key;
			tSession.date = decision->date;
			tSession.kind = decision->sessionKind;

			if (!tSession.Get())
			{
				TXBankError(decision, "липсващо заседание");
				return;
			}

			if (!Result->Xfer(tSession.result))
				return;

			session = &tSession;
		}
	}

	xfer_bank(TXferBankEvent(lawsuit, session, surrounds, decision, inReg, sideWinds, outReg, action));
}

// ----- TXferBankSessionChange ----------------------------------------------
class TXferBankSessionChange : public TXferBank
{
public:
	TXferBankSessionChange(TLawsuit *lawsuit, TSession *tPrevious, TSession *tSession, TGArray<TSurround> *tSurrounds,
		TGArray<TDecision> *tDecisions);

	virtual void EmitMain();
	virtual TRCDKeyContainer *Main();

protected:
	TSession *previous;
	TSession *session;
	TGArray<TSurround> *surrounds;
	TGArray<TDecision> *decisions;
};

TXferBankSessionChange::TXferBankSessionChange(TLawsuit *lawsuit, TSession *tPrevious, TSession *tSession,
	TGArray<TSurround> *tSurrounds, TGArray<TDecision> *tDecisions)
	: TXferBank(lawsuit), previous(tPrevious), session(tSession), surrounds(tSurrounds), decisions(tDecisions)
{
}

void TXferBankSessionChange::EmitMain()
{
	EmitStart();

	if (previous)
	{
		OpenTag("Session");
		EmitSessionData(previous, surrounds, XFER_DELETE);
		CloseTag("Session");
	}

	if (session)
		EmitSessionAddFullChain(session, surrounds, decisions);

	EmitFinal();
}

TRCDKeyContainer *TXferBankSessionChange::Main()
{
	return session ? session : previous;
}

static void xfer_bank_session_change(TLawsuit *lawsuit, TSession *session, TGArray<TSurround> *surrounds,
	TGArray<TDecision> *decisions, TSession *previous)
{
	if (xfer_bank_case_check(lawsuit))
	{
		if (!Result->Xfer(previous->result))
			previous = NULL;

		if (!Result->Xfer(session->result))
			session = NULL;

		if (previous || session)
			xfer_bank(TXferBankSessionChange(lawsuit, previous, session, surrounds, decisions));
	}
}

void xfer_bank_session_reset(TLawsuit *lawsuit, TSession *session, TSession *previous)
{
	TGArray<TDecision> decisions;
	xfer_bank_session_change(lawsuit, session, NULL, &decisions, previous);
}

void xfer_bank_session_result_change(TLawsuit *lawsuit, TSession *session, TGArray<TSurround> *surrounds,
	TGArray<TDecision> *decisions, char tResult)
{
	TSession previous;

	previous << session;
	previous.result = tResult;
	xfer_bank_session_change(lawsuit, session, surrounds, decisions, &previous);
}

// ----- TXferBankDeleteSessions ---------------------------------------------
class TXferBankDeleteSessions : public TXferBank
{
public:
	TXferBankDeleteSessions(TLawsuit *lawsuit, TGArray<TSession> *tSessions);

	virtual void EmitMain();
	virtual TRCDKeyContainer *Main() { return sessions->Count() ? (*sessions)[0] : &session; }

protected:
	TGArray<TSession> *sessions;
	TSession session;
};

TXferBankDeleteSessions::TXferBankDeleteSessions(TLawsuit *lawsuit, TGArray<TSession> *tSessions)
	: TXferBank(lawsuit), sessions(tSessions)
{
	session << (*sessions)[0];
}

void TXferBankDeleteSessions::EmitMain()
{
	EmitStart();

	while (sessions->Count())
	{
		OpenTag("Session");
		EmitSessionData((*sessions)[0], NULL, XFER_DELETE);
		CloseTag("Session");
		sessions->Remove(0);
	}

	EmitFinal();
}

void xfer_bank_delete_sessions(TLawsuit *lawsuit, TGArray<TSession> *sessions)
{
	int index = 0;

	while (index < sessions->Count())
	{
		if (Result->Xfer((*sessions)[index]->result))
			index++;
		else
			sessions->Remove(index);
	}

	if (sessions->Count() && xfer_bank_case_check(lawsuit))
		xfer_bank(TXferBankDeleteSessions(lawsuit, sessions));
}

// ----- TXferBankExport -----------------------------------------------------
class TXferBankExport : public TXferBank
{
public:
	TXferBankExport(TLawsuit *lawsuit, TGroupArray *tContainers);

	virtual void EmitMain();
	virtual TRCDKeyContainer *Main() { return lawsuit; }

protected:
	TGroupArray *containers;
};

TXferBankExport::TXferBankExport(TLawsuit *lawsuit, TGroupArray *tContainers)
	: TXferBank(lawsuit), containers(tContainers)
{
}

void TXferBankExport::EmitMain()
{
	int index = 0;
	typedef TGArray<TSurround> TSurroundArray;
	TIArray<TSurroundArray> surround_arrays;

	EmitStart();

	while (index < containers->Count())
	{
		TRCDKeyContainer *container = (TRCDKeyContainer *) (*containers)[index];

		if (container->key.type == TYPE_SURROUND)
		{
			EmitSurround((TSurround *) container, NULL, XFER_ADD);
			containers->Detach(index);
		}
		else
		{
			TGArray<TSurround> *surrounds = new TGArray<TSurround>;

			LoadSurrounds((TSession *) container, surrounds);
			surround_arrays.Add(surrounds);

			for (int i = 0; i < surrounds->Count(); i++)
				EmitSurround((*surrounds)[i], NULL, XFER_ADD);

			index++;
		}
	}

	for (index = 0; index < containers->Count(); index++)
	{
		TSession *session = (TSession *) (*containers)[index];
		TGArray<TDecision> decisions;

		try2decide(session, &decisions);
		try2surroundecide(session, &decisions);
		EmitSessionAddFullChain(session, surround_arrays[index], &decisions);
		fre2surroundecide(&decisions);
	}

	EmitFinal();
}

void xfer_bank_filter(TGroupArray *array)
{
	int index = 0;
	TSurroundment surroundment;

	while (index < array->Count())
	{
		TRCDKeyContainer *container = (TRCDKeyContainer *) (*array)[index];

		if (container->key.type == TYPE_SURROUND)
		{
			msql m;

			surroundment << container;
			surroundment.FloatCriteria(m);
			TQuery q(str(m));

			if (q.Read())
				array->Remove(index);
			else
				index++;
		}
		else if (strchr(KIND_SESSIONS, container->kind))
		{
			char result = ((TSession *) container)->result;

			if (strchr(RESULT_UNLINKEDS, result) || !Result->Xfer(result))
				array->Remove(index);
			else
				index++;
		}
		else
			array->Remove(index);
	}
}

void xfer_bank_export(TLawsuit *lawsuit, TGroupArray *containers)
{
	if (xfer_bank_case(lawsuit, NULL, NULL, XFER_ADD) && containers->Count() > 0)
		xfer_bank(TXferBankExport(lawsuit, containers));
}
