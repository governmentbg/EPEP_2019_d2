#include "cac.h"

// ----- TRequestResolution --------------------------------------------------
TRequestResolution::TRequestResolution()
	: TIndexContainer("T_REQUEST_RESOLUTION", TRequestResolution::Clone)
{
	Add(
		new TCharAlias("F_RESOLUTION", &resolution),
		new TText("F_TEXT", &text, SIZE_OF_RESOLUTION_TEXT),
		new TPersonAlias("F_JUDGE", judge, sizeof judge),
		new TPersonAlias("F_ASSIGN", assign, sizeof assign),
		new TLong("F_LAWCHOICE_NO", &lawChoiceNo),
		NULL
	);

	descs = Descs + TABLE_REQUEST_RESOLUTION;
	key.type = TYPE_REQUEST;
	kind = KIND_REQUEST_RESOLUTION;
}

TGroup *TRequestResolution::Clone()
{
	return new TRequestResolution;
}

void TRequestResolution::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_NO, F_YEAR, F_TYPE, F_DATE");
}

char TRequestResolution::IndexKind() const
{
	return KIND_ORDER;
}

char TRequestResolution::IndexType() const
{
	if (strchr(RESOLUTION_INDEXABLES, resolution))
	{
		TRequest request;
		request.key = key;

		if (request.Get("F_COLLEGE"))
			return request.college;
	}

	return TYPE_NULL;
}

void TRequestResolution::Involvements(mstr &m) const
{
	if (IndexType())
		TIndexContainer::Involvements(m);
	else
		m.clear();	// required?..
}

// ----- TRequest ------------------------------------------------------------
TRequest::TRequest()
	: TOriginContainer("T_REQUEST", TRequest::Clone), xfer(NULL)
{
	Add(
		new TString("F_TEXT", text, sizeof text),
		new TTime("F_TIME", &time),
		new TString("F_DIRECTION", direction, sizeof direction),
		new TCharAlias("F_LAW_KIND", &lawKind),
		NULL
	);

	descs = Descs + TABLE_REQUEST;
	key.type = TYPE_REQUEST;
	antiNoRight = RIGHT_ANTIDOCNO;
}

TGroup *TRequest::Clone()
{
	return new TRequest;
}

char TRequest::LawKind() const
{
	return lawKind;
}

bool TRequest::Generate()
{
	return true;
}

bool TRequest::Insert()
{
	key.year = date.year;

	if (time.Empty())
		time = LNow();

	*RequestIndex << this;
	if (!RequestIndex->Insert())
		return false;
	*this << RequestIndex;

	if (!TOriginContainer::Insert())
	{
		RequestIndex->Delete(true);
		return false;
	}

	return true;
}

TRequestResolution *TRequest::LastResolution(const char *fields) const
{
	msql m;
	TRequestResolution *resolution = new TRequestResolution;

	LastKey(m, resolution, fields);
	TQuery q(str(m));

	if (q.Read())
		*resolution << q;
	else
	{
		delete resolution;
		resolution = NULL;
	}

	return resolution;
}

bool TRequest::Latest(TRCDKeyContainer *)
{
	return false;
}

char TRequest::Status() const
{
	TRequestResolution *lastResolution = LastResolution("F_RESOLUTION");

	if (lastResolution)
	{
		char status = lastResolution->Status();
		TVirtPtr VP(lastResolution);
		return status;
	}

	return TOriginContainer::Status();
}

// ----- TWindData -----------------------------------------------------------
TWindData::TWindData(const char *name, TGroup *(*const tClone)())
	: TUCNGroup(name, tClone)
{
	Add(
		NEW_RCDKEY("F_", &key),
		new TCharAlias("F_KIND", &kind),
		new TDate("F_DATE", &date, true),
		new TCharAlias("F_INVOLVEMENT", &involvement),
		NULL
	);
}

void TWindData::CopyTo(const TRCDKeyContainer *target, bool override)
{
	kind = target->kind;
	date = target->date;
	key = target->key;

	if (!Try("F_NO"))
		Insert();
	else if (override)
	{
		Delete(true);
		Insert();
	}
}

// ----- TBaseWind -----------------------------------------------------------
const TWindIndex WIND_INDEXES[] =
{
	{ "T_SIDE_WIND",	TABLE_SIDE_WIND },
	{ "T_LINK_WIND",	TABLE_LINK_WIND },
	{ "T_SUBPOENA",	TABLE_SUBPOENA },
	{ "T_JUDGEMENT",	TABLE_JUDGEMENT },
	{ "T_EXPENSE",	TABLE_EXPENSE },
	{ NULL, -1 }
};

TBaseWind::TBaseWind(const char *name, TGroup *(*const tClone)())
	: TWindData(name, tClone)
{
	// 2013:311 FIX: for TBaseName("T_NAME")

	for (const TWindIndex *wi = WIND_INDEXES; wi->name; wi++)
	{
		if (!strcmp(wi->name, name))
		{
			descs = Descs + wi->table;
			break;
		}
	}
}

long TBaseWind::Excess() const
{
	return 0;
}

void TBaseWind::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_UCN, F_UCN_TYPE, F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_INVOLVEMENT");
}

void TBaseWind::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	// 2017:286 FIX: for 2nd intervals and bad start dates
	// 2017:339 FIX: revert, breaks renumber, no easy fix
	m.Add(this, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE");
	//m.Add(this, "F_NO, F_YEAR, F_TYPE, F_KIND");
	//if (!strchr(KIND_LAWSUITS, kind))
	//	m.Add(this, "F_DATE");
}

void TBaseWind::PrintGroup(mstr &m, bool cut)
{
	TUCNGroup *const ucnGroup = TNamedGroup::NewGroup(ucnType);
	TVirtPtr VP(ucnGroup);
	size_t pos = m.len();

	*ucnGroup << this;
	ucnGroup->Try();
	ucnGroup->Print(m);

	if (cut)
		mbk_remove(&m, pos, SIZE_OF_UCN_TYPE_UCN);
}

void TBaseWind::PrintIdent(mstr &m)
{
	if (ucnType == UCN_CITIZEN_UCN)
		m.printf(" ЕГН %s", ucn);
	else if (strchr(UCN_FIRMS, ucnType))
	{
		TFirm firm;

		firm.ucnType = ucnType;
		strcpy(firm.ucn, ucn);

		if (firm.Try("F_BULSTAT") && any(firm.bulstat))
			m.printf(" ЕИК/БУЛСТАТ %s", firm.bulstat);
	}
}

void TBaseWind::Transfer(const TRCDKeyContainer *source, const TRCDKeyContainer *target, const char *involvements,
	TBaseWind *baseWind)
{
	TChar *state = (TChar *) Find("F_STATE");		// 2009:155 +related
	TLong *flags = (TLong *) Find("F_FLAGS");		// 2016:350 +related

	// Low-level - transfer sources are calculated individually
	// Moving to the same event is a successful no-operation
#if 20170152
	if (!conteq(source, target))
#else  // 20170152
	if (source->key != target->key || source->kind != target->kind || source->date != target->date)
#endif  // 20170152
	{
		msql m;  // 2017:111 testver s[] -> msql; 2017:137 official

		if (!baseWind)
			baseWind = this;

		baseWind->key = source->key;
		baseWind->kind = source->kind;
		baseWind->date = source->date;
		baseWind->FloatCriteria(m);

		TQuery q(str(m));
		while (q.Read())
		{
			*this << q;
			key = target->key;
			kind = target->kind;
			date = target->date;

			// 2010:216 LPR/SRQ: also preserve appealers for inreg re#; 2011:175 also for surround re#
			//if (state && strchr(STATE_APPEALERS, state->C()) && !strchr(KIND_INREG_EVENTS, target->kind))
			//if (state && strchr(STATE_APPEALERS, state->C()) && target->key.type != TYPE_INREG &&
			//	target->key.type != TYPE_SURROUND)
			// 2013:070 LRQ: clear state for inReg -> surround
			if (state)
			{
				if (strchr(STATE_APPEALERS, state->C()) && source->key.type != target->key.type)
					state->SetC(STATE_NONE);
			#if COURT_TYPE == COURT_REGIONAL
				else if (strchr(STATE_HERITAGES, state->C()) && strchr(KIND_ENDOCS, source->kind))
					state->SetC(STATE_NONE);
			#endif  // REGIONAL
			}

			if (flags && !strchr(TYPE_ORIGINS, target->key.type))
				flags->SetL(flags->L() & ~SIDE_WIND_CHILD);

			if (strmatch(involvement, involvements))
				Insert();
		}
	}
}

bool TBaseWind::SameWind(const TBaseWind *that) const
{
	return ucnType == that->ucnType && involvement == that->involvement && !strcmp(ucn, that->ucn);
}

// ----- TSideWind -----------------------------------------------------------
TSideWind::TSideWind()
	: TBaseWind("T_SIDE_WIND", TSideWind::Clone)
{
	Init();
}

TSideWind::TSideWind(const char *name, TGroup *(*const tClone)())
	: TBaseWind(name, tClone)
{
	Init();
}

void TSideWind::Init()
{
	Add(
		new TChar("F_STATE", &state),
		new TLong("F_ORDER", &order),
		new TLong("F_FLAGS", &flags),
		new TCharAlias("F_FROM_SIDE", &fromSide),
		new TString("F_UNWANTED", unwanted, sizeof unwanted),
		new TLong("F_INVEST_ID", &investId),
		new TLong("F_INVEXP_ID", &invExpId),
		NULL
	);

	// descs assigned in TBaseWind
	state = STATE_NONE;
	fromSide = INVOLVEMENT_FROM_NULL;
}

TGroup *TSideWind::Clone()
{
	return new TSideWind;
}

bool TSideWind::Appealer()
{
	return involvement == INVOLVEMENT_APPEALER || strchr(STATE_APPEALERS, state) || kind == KIND_EPORTAL_REQUEST;
}

constant TSideWind::ChildText(long flags)
{
	switch (flags & SIDE_WIND_CHILD)
	{
		case SIDE_WIND_ADULT : return " ";
		case SIDE_WIND_MINOR : return "Непълнолетен";
		case SIDE_WIND_INFANT : return "Малолетен";
	}
	// ADMNU
	return "Пълнолетен";
}

void TSideWind::FloatCriteria(msql &m, const char *fields)
{
	TBaseWind::FloatCriteria(m, fields);
	m.AddOrder("F_ORDER");
}

void TSideWind::Generate()
{
	msql m;

	TBaseWind::FloatCriteria(m, "MAX(F_ORDER) AS F_ORDER");
	order = dataBase->GetLong(str(m), "F_ORDER");

	if (++order <= 0)
		generator_limit(Name);
}

bool TSideWind::Insert()
{
	if (!order)
		Generate();

	return TBaseWind::Insert();
}

unsigned TSideWind::Key() const
{
#if RANGECHECK
	if (!involvement)
		fatal("side Key for empty involvement");
#endif  // RANGECHECK
	Involvement->Seek(involvement);
	return Involvement->Order() * 0x100000 + order;
}

void TSideWind::Print(mstr &m)
{
	size_t len = m.len();

	if (involvement)
	{
		m.cat(Involvement->Seek(involvement));
		m.setlen(len + LENGTH_OF_INVOLVEMENT_DISPLAY);
		// 2007:281 LPR: separator for non-empty state
		if (state == STATE_NONE)
			m.cach(' ');
		else
		{
			ncstr(m)[len + LENGTH_OF_INVOLVEMENT_DISPLAY - 1] = ' ';
			m.cach(state);
		}
	}

	m.printf("%c ", ChildText()[0]);
	// 2008:070 separate method
	PrintGroup(m);
}

void TSideWind::PrintState(mstr &m)
{
	if (state != STATE_NONE || fromSide == INVOLVEMENT_FROM_NULL)
		m.cach(state);
	else
	{
		const char *sep = strchr(Involvement->Seek(fromSide), ' ');

		m.cach(*Involvement->name);

		if (sep)
			m.cach(sep[1]);
	}

	m.printf("%c%c", ChildText()[0], side_wind_notified_mark(this, eventStatus));
}

bool TSideWind::Represents(const TLinkWind *linkWind) const
{
	return (linkWind->redirectUcnType == ucnType && !strcmp(linkWind->redirectUcn, ucn)) ||
		(!atob(linkWind->redirectUcn) && linkWind->proxyUcnType == ucnType && !strcmp(linkWind->proxyUcn, ucn));
}

bool TSideWind::Represents(const TSideWind *sideWind, TGArray<TLinkWind> *links) const
{
	for (int i = 0; i < links->Count(); i++)
	{
		TLinkWind *linkWind = (*links)[i];

		if (linkWind->ucnType == sideWind->ucnType && !strcmp(linkWind->ucn, sideWind->ucn) && Represents(linkWind))
			return true;
	}

	return false;
}

int TSideWind::LoadSides(TRCDKeyContainer *container, TGArray<TSideWind> *left, TGArray<TSideWind> *right,
	TGArray<TSideWind> *other)
{
	msql m;
	int count = 0;

	// LPR: 99.99% real sides, not worth select criteria
	*this << container;
	FloatCriteria(m, "F_UCN, F_UCN_TYPE, F_INVOLVEMENT, F_STATE, F_FROM_SIDE, F_FLAGS");

	TQuery q(str(m));
	while (q.Read())
	{
		*this << q;

		if (strchr(UCN_REAL_SIDES, ucnType))
		{
			TGArray<TSideWind> *sideWinds;
			char category = container->Category(involvement);

			switch (category)
			{
				case SIDEWIND_LEFT : sideWinds = left; break;
				case SIDEWIND_RIGHT : sideWinds = right; break;
				default : sideWinds = other;
			}

			if (sideWinds)
			{
				sideWinds->AddCopy(this);
				count++;
			}
		}
	}

	return count;
}

// ----- TMovement -----------------------------------------------------------
TMovement::TMovement()
	: TDatedContainer("T_MOVEMENT", TMovement::Clone)
{
	Add(
		new TCharAlias("F_RESULT", &result),
		new TString("F_TEXT", text, sizeof text),
		new TTime("F_TIME", &time),
		new TString("F_UCN", ucn, sizeof ucn),
		NULL
	);

	descs = Descs + TABLE_MOVEMENT;
	key.type = TYPE_MOVEMENT;
	kind = KIND_NULL;
}

TGroup *TMovement::Clone()
{
	return new TMovement;
}

void TMovement::ExactCriteria(msql &m, const char *fields)
{
	TDatedContainer::ExactCriteria(m, fields);
	m.Add(this, "F_TIME");
}

bool TMovement::Insert()
{
	result = kind == KIND_IN_MOVEMENT ? RESULT_MOVED_IN : RESULT_MOVED_OUT;
	strcpy(ucn, Default->ucn);

	if (TGroup::Insert())
	{
		Signal(ELECTRIC_ADD);
		return true;
	}

	return false;
}

bool TMovement::operator<=(const TMovement &that) const
{
	return date != that.date ? date < that.date : time <= that.time;
}

// ----- TLawsuit ------------------------------------------------------------
TLawsuit::TLawsuit()
	: TOriginContainer("T_LAWSUIT", TLawsuit::Clone)
{
	Add(
		NEW_RCDKEY("F_SOURCE_", &source),
		new TCharAlias("F_SOURCE_KIND", &sourceKind),
		new TLongAlias("F_COMPOSITION", &composition),
		new TPersonAlias("F_JUDGE", judge, sizeof judge),
		new TCharAlias("F_AGE", &age),
		new TDate("F_FINISHED", &finished, false),
		new TCharAlias("F_GRANTED", &grant),
		new TDate("F_DEC_MADE", &decMade, false),
		new TString("F_TEXT", text, sizeof text),
		new TDate("F_RETURNED_DATE", &returnedDate, false),
		new TDate("F_PREVIOUS_DATE", &previousDate, true),
		new TDate("F_RECEIVED_DATE", &receivedDate, false),
		new TDate("F_FORCE_DATE", &forceDate, false),
		new TDate("F_COMPLETED", &completed, false),
		new TLong("F_FLAGS", &flags),
		new TCheck("F_FLAGS_IMPORTANT", &flags, LAWSUIT_IMPORTANT),
		new TText("F_IMPORTANT", &important, SIZE_OF_LAWSUIT_IMPORTANT),
		new TPersonAlias("F_REJUDGE", rejudge, sizeof rejudge),
		new TCheck("F_QUICK_QUICK", &quick, QUICK_QUICK),
		new TCheck("F_QUICK_CONVERTED", &quick, QUICK_CONVERTED),
		new TCheck("F_FLAGS_NEWGPK", &flags, LAWSUIT_NEWGPK),
		new TCheck("F_FLAGS_LIMITED", &flags, LAWSUIT_LIMITED),
		new TLongAlias("F_DANGER", &danger),
		new TDate("F_FIRST_SET", &firstSet, false),
		new TLong("F_CUTDOWN", &cutDown),
		new TLong("F_FLAGS_X", &flagsX),
		new TCheck("F_FLAGS_COMPLEX", &flagsX, LAWSUIT_X_COMPLEX),
		new TCheck("F_FLAGS_DOUBLEX", &flagsX, LAWSUIT_X_DOUBLEX),
		new TCheck("F_FLAGS_CHILDREN", &flagsX, LAWSUIT_X_CHILDREN),
		new TCheck("F_FLAGS_CORRUPT", &flags, LAWSUIT_CORRUPT),
		new TLong("F_FLAGS_Y", &flagsY),		
		new TCheck("F_FLAGS_PT329A3", &flagsY, LAWSUIT_Y_PT329A3),
		NULL
	);

	descs = Descs + TABLE_LAWSUIT;
	antiNoRight = RIGHT_ANTILAWNO;
	composition = Default->compos;
}

TGroup *TLawsuit::Clone()
{
	return new TLawsuit;
}

bool TLawsuit::Generate()
{
	return EXTERNAL_LAWSUIT_NO || TOriginContainer::Generate();
}

bool TLawsuit::Insert()
{
	if (!date.year)
		date = Today;
	//if (!key.year)	// next line was unreachable
		key.year = date.year;
	if (!source.type)
		source.type = kind2type(sourceKind);
	if (!age)
		age = AGE_NONE_YET;
	if (!grant)
		grant = GRANT_NONE_YET;

	if (!atob(judge))
		*judge = '\0';
	else if (!atob(rejudge))
		*rejudge = '\0';

	strcpy(rejudge, judge);

	if (!TOriginContainer::Insert())
		return false;

	TLawInterval interval;

	interval << *this;
	interval.flags = 0;	// override law flags
	interval.Insert();
	return true;
}

void TLawsuit::Involvements(mstr &m) const
{
	kind2involvements(sourceKind, m);
	TOriginContainer::Involvements(m);
	// 2009:005 LPR: fixup
	if (key.type != TYPE_PUNISHMENT_LAW)
	#if 20170228
		for (const char *inv = INVOLVEMENT_PUNISHMENTS; *inv; inv++)
			m.rep(*inv, INVOLVEMENT_INVALID);
	#else  // 20170228
	{
		char *pos;

		for (const char *inv = INVOLVEMENT_PUNISHMENTS; *inv; inv++)
			if ((pos = strchr(s, *inv)) != NULL)
				*pos = INVOLVEMENT_INVALID;
	}
	#endif  // 20170228
}

bool TLawsuit::Latest(TRCDKeyContainer *container)
{
	if (source.no)
	{
		container->key = source;
		container->date = date;
		return TOriginContainer::Latest(container);
	}

	return false;
}

TExternContainer *TLawsuit::NewGroup(long excess)
{
	return excess ? (TExternContainer *) new TExternLawsuit : (TExternContainer *) new TLawsuit;
}

class TLawsuitStatus
{
public:
	CDate date;
	char kind;
	char status;

	void Process(const char *s, TDatedContainer *container, CDate &container_date);
};

void TLawsuitStatus::Process(const char *s, TDatedContainer *container, CDate &container_date)
{
	TQuery q(s);
	while (q.Read())
	{
		*container << q;
		if (date.Empty())
		{
			date = container_date;
			status = container->Status();
			kind = container->kind;
		}
		else
		{
			if (container_date < date)
				break;

			if (
				container_date > date ||
				strchr(KIND_SESSIONS_ENDOCS, container->kind) > strchr(KIND_SESSIONS_ENDOCS, kind))
			{
				date = container_date;
				status = container->Status();
				kind = container->kind;
			}
		}
	}
}

bool TLawsuit::PostPersonal() const
{
	Subject->Seek(key.type, subject);
	return strchr(Subject->Attrib(kind), POST_PERSONAL);
}

char TLawsuit::Status() const
{
	msql m;
	TSession session;
	TDecision decision;
	TLawsuitStatus status;

	LastKey(m, &session, "F_KIND, F_DATE, F_RESULT");
	status.Process(str(m), &session, session.date);

	m.Begin(decision.Name, "F_KIND, F_FROM_DATE, F_RESULT");
	m.Add(this, "F_NO, F_YEAR, F_TYPE");
	m.AddOrder("F_FROM_DATE DESC");

	status.Process(str(m), &decision, decision.fromDate);
	return status.date.Empty() ? TOriginContainer::Status() : status.status;
}

bool TLawsuit::Suitor(char involvement)
{
	return TOriginContainer::Suitor(involvement) || is_suitor_ll(sourceKind, involvement);
}

bool TLawsuit::TryBy(const TRCDKey &source, const char *fields)
{
	msql m(Name, fields);

	m.AddLong("F_SOURCE_NO", source.no);
	m.AddLong("F_SOURCE_YEAR", source.year);
	m.AddChar("F_SOURCE_TYPE", source.type);

	TQuery q(str(m));
	if (q.Read())
	{
		*this << q;
		return true;
	}

	return false;
}

bool TLawsuit::Update(const char *fields)
{
	if (!atob(judge))
		*judge = '\0';
	else if (!atob(rejudge))
		*rejudge = '\0';
	else if (!strcmp(judge, INTER))
		strcpy(judge, rejudge);
	else if (!strcmp(rejudge, INTER))
		strcpy(rejudge, judge);

	return TOriginContainer::Update(fields);
}

// ----- TBaseSession --------------------------------------------------------
TBaseSession::TBaseSession(const char *name, TGroup *(*const tClone)())
	: TIndexContainer(name, tClone)
{
	Add(
		new TTime("F_TIME", &time),
		new TDate("F_END", &end, false),
		new TTime("F_ENTIME", &entime),
		new TLongAlias("F_COMPOSITION", &composition),
		new TPersonAlias("F_JUROR", juror, sizeof juror),
		new TPersonAlias("F_REJUROR", reJuror, sizeof reJuror),
		new TCharAlias("F_RESULT", &result),
		new TPersonAlias("F_JUROR_X", jurorX, sizeof jurorX),
		new TPersonAlias("F_JUDGE", judge, sizeof judge),
		new TPersonAlias("F_SECRETAR", secretar, sizeof secretar),
		new TPersonAlias("F_RESERVE", reserve, sizeof reserve),
		NULL
	);
}

const char *TBaseSession::ExcessText(long excess)
{
	return excess ? "С" : "";
}

void TBaseSession::GetLast(const char *fields)
{
	msql m;
	TBaseSession *session = (TBaseSession *) clone();
	TVirtPtr VP(session);

	LastKey(m, session, fields, KIND_OPEN_SESSIONS);
	TQuery q(str(m));

	if (q.Read())
      {
      	*session << q;
		ImportFields(session, fields);
      }
}

TBaseSession *TBaseSession::NewGroup(long excess)
{
	return excess ? (TBaseSession *) new TExternSession : (TBaseSession *) new TSession;
}

// ----- TSession ------------------------------------------------------------
TSession::TSession(TGroup *(*const tClone)())
	: TBaseSession("T_SESSION", tClone)
{
	Add(
		new TPersonAlias("F_PROSECUTOR", prosecutor, sizeof prosecutor),
		new TPersonAlias("F_JUDGE_X", judgeX, sizeof judgeX),
		new TPersonAlias("F_REJUDGE_X", reJudgeX, sizeof reJudgeX),
		new TPersonAlias("F_PRESIDENT", president, sizeof president),
		new TDate("F_RETURNED", &returned, false),
		new TText("F_TEXT", &text, SIZE_OF_SESSION_TEXT),
		new TLongAlias("F_HALL", &hall),
		new TLong("F_FLAGS", &flags),
		new TCheck("F_FLAGS_PROCEED", &flags, SESSION_PROCEED),
		new TCheck("F_FLAGS_CHANJED", &flags, SESSION_CHANJED),
		new TDate("F_FINAL", &final, false),
		new TCheck("F_FLAGS_EXPERT", &flags, SESSION_EXPERT),
		new TCheck("F_JURORS", &jurors, TRUE),
		new TCheck("F_FLAGS_PRIVATE", &flags, SESSION_PRIVATE),
		new TDate("F_FORCE_DATE", &forceDate, false),
		new TTime("F_STARTIME", &startime),
		new TLongAlias("F_SLC_REASON", &slcReason),
		new TLong("F_FLAGS_X", &flagsX),
		new TCheck("F_FLAGS_CLOSEDD", &flagsX, SESSION_X_CLOSEDD),
		new TCheck("F_FLAGS_FIRSET", &flagsX, SESSION_X_FIRSET),
		new TCheck("F_FLAGS_APPEAL", &flagsX, SESSION_X_APPEAL),
		new TCheck("F_FLAGS_RECUJED", &flagsX, SESSION_X_RECUJED),
		new TLong("F_FLAGS_Y", &flagsY),
		new TCheck("F_FLAGS_PUBLTREG", &flagsY, SESSION_Y_PUBLTREG),
		new TCheck("F_FLAGS_RESTART", &flagsY, SESSION_Y_RESTART),
		NULL
	);

	descs = Descs + TABLE_SESSION;
}

TGroup *TSession::Clone()
{
	return new TSession;
}

const char *TSession::Accomplys() const
{
	const char *accomplys = ACCOMPLY_NULLS;
#if COURT_TYPE == COURT_AREAL
	switch (result)
	{
		case RESULT_CANCELED : accomplys = ACCOMPLY_CANCELEDS; break;
		case RESULT_SEND : accomplys = ACCOMPLY_SENDS; break;
		default :
		{
			if (strchr(RESULT_INCONSISTENCES, result))
				accomplys = ACCOMPLY_DECIDEDS;
		}
	}
#endif  // AREAL
	return accomplys;
}

bool TSession::DefaultPublic() const
{
	return strchr(RESULT_COMPLETIZEDS, result) || result == RESULT_EXPLICIT_BUGFIX;
}

// 2009:224 LPR: new implementation with target buffer
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
const char *TSession::DecisionKinds(mstr &m, const TLawsuit *lawsuit)
#endif  // AREAL || REGIONAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY
const char *TSession::DecisionKinds(mstr &m, const TLawsuit *)
#endif  // APPEAL || ADMIN || MILITARY
{
	m.clear();

	if (strchr(RESULT_DECIDABLES, result))
	{
		m.cat(KIND_DECISIONS);
	#if COURT_TYPE == COURT_REGIONAL
		if (lawsuit->sourceKind == KIND_EUHER_REQUEST)
			m.cach(KIND_EUHER_CERT);
	#endif  // REGIONAL
	}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (strchr(RESULT_JUDGEMENTABLES, result))
		m.cach(KIND_JUDGEMENT);
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_APPEAL
	if (strchr(RESULT_SETTLEMENTABLES, result) || strchr(RESULT_JUDGEMENTABLES, result))
		m.cach(KIND_SETTLEMENT);
#endif  // APPEAL

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (strchr(RESULT_BULWARKABLES, result))
		m.cach(KIND_BULWARK);

	if (strchr(RESULT_SETTLEMENTABLES, result) || strchr(RESULT_JUDGEMENTABLES, result) || strchr(RESULT_BULWARKABLES, result))
		m.cach(KIND_SETTLEMENT);
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (strchr(RESULT_ORDERABLES, result))
		m.cach(KIND_ORDER);
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

	if (strchr(RESULT_PROTOCOLABLES, result))
		m.cach(KIND_PROTOCOL);

#if COURT_TYPE == COURT_ADMIN
	if (strchr(RESULT_SETTLEMENTABLES, result))
		m.cach(KIND_SETTLEMENT);

	if (strchr(RESULT_ORDERABLES, result))
		m.cach(KIND_ORDER);
#endif  // ADMIN

	// 2014:255
#if COURT_TYPE == COURT_AREAL
	if (strchr(KIND_2ND_CITIZEN_LAWS, lawsuit->kind))
		m.cach(KIND_PROTECTIVE);
#endif

	// 2008:058; 2008:295
#if COURT_TYPE == COURT_REGIONAL
	// 2010:065 LRQ: YA special exception
	if (result == RESULT_FORCE_EXECUTIVE)
		m.cach(KIND_EXECUTIVE);
	else if (result == RESULT_IMMED_PROTECT)
		m.cach(KIND_IMMED_PROTECT);
	else
	{
		bool prot = strchr(RESULT_PROTECTABLES, result);
		bool exec = strchr(RESULT_EXECUTIVABLES, result);

		if (exec || prot)
		{
			if (prot && lawsuit->sourceKind == KIND_PROTECTIVE_REQUEST)
			{
				m.cach(KIND_SETTLEMENT);
				m.cach(KIND_PROTECTIVE);
			}
			else if (exec && lawsuit->sourceKind == KIND_EXECUTIVE_REQUEST)
				m.cach(KIND_EXECUTIVE);
		}
		// 2017:263
		if (result == RESULT_WITH_SETTLEMENT && lawsuit->sourceKind == KIND_EUHER_REQUEST)
			m.cach(KIND_EUHER_CERT);
	}
#endif  // REGIONAL

#if COURT_TYPE == COURT_AREAL
	if (strchr(KIND_2ND_LAWSUITS, lawsuit->kind) && result == RESULT_CANCEL_SETTLED)
		m.cach(KIND_DECISION);
#endif  // AREAL

	// 2011:336 LRQ: comout
	//if (result == RESULT_WITH_DECREE)
	//	m.cach(KIND_DECREE);

	// 2017:157
	if (result == RESULT_CANCELED)
		m.cach_nx(KIND_DECISION);

	return str(m);
}

long TSession::Excess() const
{
	return 0;
}

char TSession::IndexKind() const
{
	// 2011:336 LRQ: decree; 2013:071 non-joinder
	switch (result)
	{
		case RESULT_WITH_DECREE : return KIND_DECREE;
		case RESULT_WITH_ORDER :
		case RESULT_NON_JOIN_ORDER :
		case RESULT_HOLD_ORD : return KIND_ORDER;
	}

	return KIND_SETTLEMENT;
}

char TSession::IndexType() const
{
	if (strchr(KIND_OPEN_SESSIONS, kind))
		return TYPE_NULL;

	if (strchr(RESULT_ENTERABLES, result))
		return TYPE_NULL;

	return key.type;
}

bool TSession::Insert()
{
	if (!atob(judge))
		*judge = '\0';

	if (end == date)
		end.Clear();

	return TBaseSession::Insert();
}

bool TSession::IsEPortSession() const
{
	if (flags & SESSION_PRIVATE)
		return false;

	return strchr(KIND_OPEN_SESSIONS, kind) || strchr(RESULT_ENTERABLES, result);
}

bool TSession::IsEPortSessEndoc() const
{
	if (strchr(RESULT_UNCARRIEDS, result) || (flags & SESSION_PRIVATE))
		return false;

	return !strchr(RESULT_ENTERABLES, result);
}

bool TSession::PersonalData()
{
	if (!strchr(key.type == TYPE_PUNISHMENT_LAW ? POST_CRIME_ATTRIBS : POST_OTHER_ATTRIBS, POST_PERSONAL))
		return false;

	TLawsuit lawsuit;
	lawsuit.key = key;

	return lawsuit.Get("F_KIND, F_SUBJECT") && lawsuit.PostPersonal();
}

bool TSession::Update(const char *fields)
{
	if (!atob(judge))
		*judge = '\0';

	return TBaseSession::Update(fields);
}

// ----- TCitizenNames -------------------------------------------------------
TCitizenNames::TCitizenNames(TGroup *self)
{
	if (!self->Find("F_NAME"))
		self->Add(new TName("F_NAME", name, sizeof name));

	self->Add(
		new TName("F_FAMILY", family, sizeof family),
		new TName("F_RENAME", reName, sizeof reName),
		new TName("F_REFAMILY", reFamily, sizeof reFamily),
		new TName ("F_RANK", rank, sizeof rank),
		NULL
	);
}

const char *TCitizenNames::Rank() const
{
	strcpy(rank_, rank);
	if (any(rank_))
		strcat(rank_, " ");

	return rank_;
}

bool TCitizenNames::CheckLiterate()
{
	static constant users[] = { "Името", "Презимето", "Фамилията", "Втората фамилия", NULL };
	constant names[] = { name, reName, family, reFamily, NULL };
	unsigned total = 0;

	for (int i = 0; names[i]; i++)
	{
		unsigned flags = 0;

		for (const char *s = names[i]; *s; s++)
			if (isalpha(*s))
				flags |= islocal(*s) ? 1 : 2;

		if (flags == 3 && !warn("%s съдържа символи на кирилица и латиница. Желаете ли да продължите?", users[i]))
			return false;

		total |= flags;
	}

	return total != 3 || warn("Имената са смесени, на кирилица и латиница. Желаете ли да продължите?"); 
}

// ----- TCitizen ------------------------------------------------------------
TCitizen::TCitizen()
	: TNamedGroup("T_CITIZEN", TCitizen::Clone, new TName("F_NAME", name, sizeof name)), TCitizenNames(this)
{
	Init();
}

TCitizen::TCitizen(const char *tName, TGroup *(*const tClone)())
	: TNamedGroup(tName, tClone, new TName("F_NAME", name, sizeof name)), TCitizenNames(this)
{
	Init();
}

void TCitizen::Init()
{
	Add(
		new TStringAlias("F_COUNTRY", country, sizeof country),
		new TStringAlias("F_RECOUNTRY", reCountry, sizeof reCountry),
		new TString("F_LNC", lnc, sizeof lnc),
		NULL
	);

	descs = Descs + TABLE_CITIZEN;
	strcpy(country, BUILDIN_COUNTRY);
}

TGroup *TCitizen::Clone()
{
	return new TCitizen;
}

bool TCitizen::Delete(bool exact)
{
	if (!strstr(Name, "_OLD_") && exact && ucnType == UCN_CITIZEN_UCN && is_internal(ucn))
		return error("Лицето е забранено за изтриване.");

	return TUCNGroup::Delete(exact);
}

void TCitizen::NamedCriteria(msql &m, const char *fields, const char *extra, char involvement, bool active)
{
	m.Begin(Name, fields);

	m.AddExtra("F_NAME", name, extra);
	m.AddExtra("F_FAMILY", family, extra);
	m.AddLike("F_RENAME", reName, false);
	m.AddExtra("F_LNC", lnc, extra);
	KnownCriteria(m, involvement, active);
	m.AddOrder("F_NAME, F_RENAME, F_FAMILY");
}

void TCitizen::Print(mstr &m)
{
	TUCNGroup::Print(m);

	if (any(reName))
		m.printf(" %s%s %s %s", Rank(), name, reName, family);
	else
		m.printf(" %s%s %s", Rank(), name, family);

	if (any(reFamily))
		m.printf(" %s", reFamily);
}

bool TCitizen::Update(const char *fields)
{
	if (!strstr(Name, "_OLD_")  && ucnType == UCN_CITIZEN_UCN && is_internal(ucn))
		return error("Лицето е забранено за модификация и ползване.");

	return TUCNGroup::Update(fields);
}

bool TCitizen::ValidCriteria()
{
	return (any(name) && any(family)) || any(lnc);
}

// ----- TFirm ---------------------------------------------------------------
TFirm::TFirm()
	: TNamedGroup("T_FIRM", TFirm::Clone, new TJudicialName("F_NAME", name, sizeof name))
{
	Init();
}

TFirm::TFirm(const char *tName, TGroup *(*const tClone)())
	: TNamedGroup(tName, tClone, new TJudicialName("F_NAME", name, sizeof name))
{
	Init();
}

void TFirm::Init()
{
	Add(
		new TString("F_UFN", ufn, sizeof ufn),
		new TString("F_BULSTAT", bulstat, sizeof bulstat),
		new TStringAlias("F_COUNTRY", country, sizeof country),
		NULL
	);

	descs = Descs + TABLE_FIRM;
	strcpy(country, BUILDIN_COUNTRY);
}

TGroup *TFirm::Clone()
{
	return new TFirm;
}

void TFirm::NamedCriteria(msql &m, const char *fields, const char *extra, char involvement, bool active)
{
	m.Begin(Name, fields);
	m.AddLike("F_BULSTAT", bulstat, false);
	m.AddLike("F_NAME", name, any(extra));
	KnownCriteria(m, involvement, active);
	m.AddOrder("F_NAME");
}

void TFirm::Print(mstr &m)
{
	TUCNGroup::Print(m);
	m.printf(" %s", name);
}

bool TFirm::ValidCriteria()
{
	return strlen(bulstat) >= 9 || strlen(name) >= 3;
}

// ----- TDamn ---------------------------------------------------------------
TDamn::TDamn()
	: TNamedGroup("T_DAMN", TDamn::Clone, new TJudicialName("F_NAME", name, sizeof name))
{
	Add
	(
		new TCharAlias("F_COMPANY_TYPE", &companyType),
		new TString("F_TAX_NO", taxNo, sizeof taxNo),
		new TString("F_BULSTAT", bulstat, sizeof bulstat),
		NEW_RCDKEY("F_", &key),
		new TDate("F_REREGISTER", &reRegister, false),
		NULL
	);

	descs = Descs + TABLE_DAMN;
	companyType = COMPANY_TYPE_DEFAULT;
}

TGroup *TDamn::Clone()
{
	return new TDamn;
}

void TDamn::NamedCriteria(msql &m, const char *fields, const char *extra, char, bool)
{
	m.Begin(Name, fields);
	m.AddLike("F_NAME", name, any(extra));
	m.AddOrder("F_NAME");
}

void TDamn::Print(mstr &m)
{
	TUCNGroup::Print(m);
	m.printf(" %s", name);
}

bool TDamn::ValidCriteria()
{
	return strlen(name) >= 3;
}

// ----- TSender -------------------------------------------------------------
TSender::TSender()
	: TNamedGroup("T_SENDER", TSender::Clone, new TName("F_NAME", name, sizeof name))
{
	Init();
}

TSender::TSender(TKnownPerson *person)
	: TNamedGroup("T_SENDER", NULL, new TName("F_NAME", name, sizeof name))
{
	Init();

	// 2011:147 NOTARY support
	if (person->involvement == INVOLVEMENT_NOTARY)
		ucnType = UCN_NOTARY;
	else
		ucnType = person->ucnType == UCN_CITIZEN_UCN ? UCN_EXECUTOR_UCN : UCN_EXECUTOR_CODE;

	strcpy(ucn, person->ucn);

	mstr m;
	person->Print(m);
	strzcpy(name, str(m), sizeof name);
}

void TSender::Init()
{
	Add(
		new TLongAlias("F_COURT", &court),
		new TLong("F_EISPP", &eispp),
		new TLong("F_FLAGS", &flags),
		NULL
	);

	descs = Descs + TABLE_SENDER;
}

TGroup *TSender::Clone()
{
	return new TSender;
}

void TSender::NamedCriteria(msql &m, const char *fields, const char *extra, char, bool)
{
	m.Begin(Name, fields);
	m.AddLike("F_NAME", name, any(extra));
	m.AddChar("F_UCN_TYPE", ucnType);
	m.AddOrder("F_NAME");
}

void TSender::Print(mstr &m)
{
	TUCNGroup::Print(m);
	m.printf(" %s", name);
}

bool TSender::ValidCriteria()
{
	return strlen(name) >= 2;
}

// ----- TReceiver -----------------------------------------------------------
TReceiver::TReceiver()
	: TStringAliasGroup("T_SENDER", TReceiver::Clone, "F_UCN", ucn, sizeof ucn, "F_NAME", name, sizeof name,
		new TCharAlias("F_UCN_TYPE", &ucnType), NULL)
{
	descs = Descs + TABLE_SENDER;
}

TGroup *TReceiver::Clone()
{
	return new TReceiver;
}

void TReceiver::AliasCriteria(msql &m, const char *xCrit, long flags)
{
	TStringAliasGroup::AliasCriteria(m, xCrit, flags);
#if 20160026
	if (any(name))
	{
		const char *s;

		for (s = name; *s; s++)
			if (!isalpha(*s))
				break;

		if (*s)
			m.AddLike("F_NAME", name, true);
		else
		{
			m.AddLike("(F_NAME", name, true);

			m.NextOr();
			strupr(name);
			m.AddLike("F_NAME", name, true);

			m.NextOr();
			strlwr(name + 1);
			m.AddLike("F_NAME", name, true);

			m.NextOr();
			*name = tolower(*name);
			m.AddLike("F_NAME", name, true);

			m.cat(")");
		}
	}
#else  // 20160026
	m.AddLike("F_NAME", name, true);
#endif  // 20160026
}

TReceiver *Receiver = NULL;

// ----- TFixeiver -----------------------------------------------------------
TFixeiver::TFixeiver(char tUCNType)
{
	ucnType = tUCNType;
	memset(ucns, '\0', sizeof ucns);
}

void TFixeiver::AliasCriteria(msql &m, const char *xCrit, long flags)
{
	TStringAliasGroup::AliasCriteria(m, xCrit, flags);
	m.Add("F_UCN");

	int count;
	for (count = 0; count < FIXEIVER_UCNS_MAX && ucns[count]; count++);

	if (count == 1)
		m.printf("= '%s'", ucns[0]);
	// 2007:064 FIX: check count, there may be no UCNs at all
	else if (count)
	{
		m.cat("IN (");

		for (int index = 0; index < count; index++)
		{
			if (index)
				m.cat(", ");
			m.printf("'%s'", ucns[index]);
		}

		m.cach(')');
	}
	else	// 2011:346 LPR: valid criteria, block
		m.cat("= ''");
}

// ----- TAnyAddress ---------------------------------------------------------
TAnyAddress::TAnyAddress(const char *name, TGroup *(*const tClone)())
	: TUCNGroup(name, tClone)
{
	Add(
		new TLong("F_ADDRESS", &address),
		new TString("F_LINE_1", line1, sizeof line1),
		new TString("F_LINE_2", line2, sizeof line2),
		new TString("F_LINE_3", line3, sizeof line3),
		new TString("F_EXTRA", extra, sizeof extra),
		NULL
	);
}

void TAnyAddress::ExactCriteria(msql &m, const char *fields)
{
	TUCNGroup::ExactCriteria(m, fields);
	m.Add(this, "F_ADDRESS");
}

void TAnyAddress::FloatCriteria(msql &m, const char *fields)
{
	TUCNGroup::ExactCriteria(m, fields);
}

void TAnyAddress::Generate()
{
	fatal("%s: invalid generator call", Name);
}

void TAnyAddress::Generate(const char *minMax)
{
	msql m;
	mstr fields;

	fields.printf("%s(F_ADDRESS) AS F_ADDRESS", minMax);
	m.Begin(Name, str(fields));
	m.AddString("F_UCN", ucn);
	m.AddChar("F_UCN_TYPE", ucnType);
	address = dataBase->GetLong(str(m), "F_ADDRESS");
}

void TAnyAddress::LoadAddresses(TUCNGroup *ucnGroup, TGSArray<TAnyAddress> *addresses, bool foreign)
{
	TAnyAddress *const address = NewGroup(foreign);
	TVirtPtr VP(address);

	*address << ucnGroup;
	address->LoadFloat(addresses);
}

void TAnyAddress::LoadAddresses(TUCNGroup *ucnGroup, TGSArray<TAnyAddress> *addresses)
{
	LoadAddresses(ucnGroup, addresses, false);
	LoadAddresses(ucnGroup, addresses, true);
}

void TAnyAddress::MakeExtra(long)
{
	*extra = '\0';
}

TAnyAddress *TAnyAddress::NewGroup(bool foreign)
{
	return foreign ? (TAnyAddress *) new TForeignAddress : (TAnyAddress *) new TAddress;
}

void TAnyAddress::Print(mstr &m, long flags)
{
	const char *lines[4] = { line1, line2, line3, extra };
	size_t len = m.len();

	MakeLines(flags);
	MakeExtra(flags);

	for (int i = 0; i < 4; i++)
	{
		if (any(lines[i]))
		{
			// 2013:245 LPR: m may be dirty
			if (m.len() > len)
				m.cat("  ");
			m.cat(lines[i]);
		}
	}
}

void TAnyAddress::Print(mstr &m)
{
	Print(m, ADDRESS_EXTRAS);
}

void TAnyAddress::PrintExtra(long flags)
{
	DeleteExtraText();

	mstr m;
	Print(m, flags);
	AddExtraText(str(m));
}

// ----- TAddressData --------------------------------------------------------
void TAddressData::AddFields(TGroup *self)
{
	self->Add(
		new TLongAlias("F_UCLP", &uclp),
		new TLongAlias("F_STREET", &street),
		new TString("F_ADR_NO", adrNo, sizeof adrNo),
		new TString("F_SUB_NO", subNo, sizeof subNo),
		new TString("F_ENTRANCE", entrance, sizeof entrance),
		new TString("F_FLOOR", floor, sizeof floor),
		new TString("F_APARTMENT", apartment, sizeof apartment),
		new TString("F_BLOCK_NO", blockNo, sizeof blockNo),
		NULL
	);
}

void TAddressData::GetUCLP()
{
	if (uclp)
	{
		UCLP->uclp = uclp;

		if (!UCLP->Get())
		{
			UCLP->area = DEFAULT_AREA;
			UCLP->region = DEFAULT_REGION;
			uclp = DEFAULT_UCLP;
			street = 0;
		}
	}
}

// ----- TAddress ------------------------------------------------------------
TAddress::TAddress()
	: TAnyAddress("T_ADDRESS", TAddress::Clone)
{
	AddFields(this);

	Add(
		new TString("F_PHONE", phone, sizeof phone),
		new TString("F_TELEX", telex, sizeof telex),
		new TString("F_FAX", fax, sizeof fax),
		new TString("F_EMAIL", email, sizeof email),
		new TString("F_REMARK", remark, sizeof remark),
		NULL
	);

	descs = Descs + TABLE_ADDRESS;
	uclp = COURT_UCLP;
	address = 1;
}

TGroup *TAddress::Clone()
{
	return new TAddress;
}

bool TAddress::Insert()
{
	Generate("MAX");

	if (++address > ADDRESS_NO_MAX)
		generator_limit(Name);

	return TAnyAddress::Insert();
}

void TAddress::MakeLines(long flags)
{
	if (uclp)
	{
		*line1 = '\0';
		UCLP->uclp = Street->uclp = uclp;
		if (!UCLP->Get())
			sprintf(UCLP->name, "гр./с.#%ld", UCLP->uclp);
		else if (!(flags & ADDRESS_SKIP_LN1))
		{
			Area->area = Region->area = UCLP->area;
			if (!Area->Get())
				sprintf(Area->name, "#%ld", Area->area);
			Region->region = UCLP->region;
			if (!Region->Get())
				sprintf(Region->name, "#%ld", Region->region);

			// 2011:003
			if (flags & ADDRESS_DISTINCT)
			{
				char name[SIZE_OF_UCLP_NAME];

				filter_prefixes(name, UCLP->name, FILTER_UCLP_PREFIXES);
				if (!strcmp(name, Region->name))
					*Region->name = '\0';
				if (!strcmp(name, Area->name))
					*Area->name = '\0';
			}

			if (any(Area->name) && any(Region->name))
			{
				int len1 = strlen(Area->name) + strlen(Region->name) + 16;
				char *format;

				if (len1 < sizeof line1)
					format = "Област %s  Община %s";
				else if (len1 == sizeof line1)
					format = "Област %s Община %s";
				else
					format = "Обл.%s Общ.%s";
				sprintf(line1, format, Area->name, Region->name);
			}
			else if (any(Area->name))
				sprintf(line1, "Област %s", Area->name);
			else if (any(Region->name))
				sprintf(line1, "Община %s", Region->name);
		}

		strcpy(line2, flags & ADDRESS_SKIP_UCLP ? "" : UCLP->name);

		if (street)
		{
			if (*line2)
				strcat(line2, "  ");

			Street->street = street;
			
			if (Street->Get())
				strcat(line2, Street->name);
			else
				scprintf(line2, "ул.#%ld", Street->street);
		}
	}
	else
		*line1 = *line2 = '\0';

	*line3 = '\0';

	struct TAddressPart
	{
		const char *name;
		const char *data;
	};

	TAddressPart ap[] =
	{
		{ "№ ", adrNo },
		{ "Блок ", blockNo },
		{ "Подн.", subNo },
		{ "Вх.", entrance },
		{ "Ет.", floor },
		{ "Ап.", apartment },
		{ NULL, NULL }
	};

	for (TAddressPart *p = ap; p->name; p++)
		if (any(p->data))
			scprintf(line3, "  %s%s" + (*line3 ? 0 : 2), p->name, p->data);
}

void TAddress::MakeExtra(long flags)
{
	// 2012:030 LRQ/URQ: rewritten to display as much as possible
	mstr m(line3);

	struct TAddressExtra
	{
		char *name;
		char *data;
		long flags;
	};

	TAddressExtra extras[] =
	{
		{ "", remark, ADDRESS_REMARK },
		{ "Тел.", phone, ADDRESS_PHONES },
		{ "Телекс", telex, ADDRESS_PHONES },
		{ "Факс", fax, ADDRESS_PHONES },
		{ "E-Mail:", email, ADDRESS_ECOMMS },
		{ NULL, NULL, 0 }
	};

	for (TAddressExtra *x = extras; x->name; x++)
	{
		if ((flags & x->flags) && any(x->data))
		{
			m.sep("  ");

			if (any(x->name))
				m.printf("%s ", x->name);
			m.cat(x->data);
		}
	}

	const char *s, *p = NULL;

	for (s = str(m); *s && s - str(m) < sizeof line3; s++)
		if (isspace(*s))
			p = s;

	s = str(m);
	if (p)
	{
		memcpy(line3, s, p - s);
		line3[p - s] = '\0';
	}
	else
		strzcpy(line3, s, sizeof line3);

	m.delead(strlen(line3));
	// m.data may be changed by remove
	for (s = str(m); isspace(*s); s++);
	strzcpy(extra, s, SIZE_OF_ADDRESS_LINE);
}

const char *TAddress::Match(int addrMode) const
{
	const char *result;

	switch (addrMode)
	{
		case ADDRESS_REQUIRE_PHONE : result = any(phone) ? NULL : "Телефона е задължителен"; break;
		case ADDRESS_REQUIRE_EMAIL : result = any(email) ? NULL : "Електронната поща е задължителна"; break;
		default : result = NULL;
	}

	return result;
}

// ----- TForeignAddress -----------------------------------------------------
TForeignAddress::TForeignAddress()
	: TAnyAddress("T_FOREIGN_ADDRESS", TForeignAddress::Clone)
{
	Add(
		new TStringAlias("F_COUNTRY", country, sizeof country),
		new TString("F_FOREIGN", foreign, sizeof foreign),
		NULL
	);

	descs = Descs + TABLE_FOREIGN_ADDRESS;
	address = -1;
}

TGroup *TForeignAddress::Clone()
{
	return new TForeignAddress;
}

bool TForeignAddress::Insert()
{
	Generate("MIN");

	if (--address < -99)
		generator_limit(Name);

	return TAnyAddress::Insert();
}

void TForeignAddress::MakeLines(long)
{
	strcpy(line1, Country->Seek(country));

	if (strlen(foreign) <= LENGTH_OF_ADDRESS_LINE)
	{
		strcpy(line2, foreign);
		*line3 = '\0';
	}
	else
	{
		char *bp = NULL;
		int found = 0;

		for (char *s = foreign + LENGTH_OF_ADDRESS_LINE; s >= foreign; s--)
		{
			if (isspace(*s) && strlen(s + 1) <= LENGTH_OF_FOREIGN_SPLIT)
			{
				bp = s;
				if (bp - foreign <= LENGTH_OF_FOREIGN_SPLIT)
					break;
			}
		}

		if (bp)
			found = 1;
		else
			bp = foreign + LENGTH_OF_FOREIGN_SPLIT;

		memcpy(line2, foreign, bp - foreign);
		line2[bp - foreign] = '\0';
		strcpy(line3, bp + found);	// skip an isspace if found
	}
}

// ----- TTryAddress ---------------------------------------------------------
TTryAddress::TTryAddress(TAnyAddress *tAddress)
	: address(tAddress), triedAddress(address != NULL)
{
}

void TTryAddress::TryAddress(const char *ucn, char ucnType)
{
	if (!triedAddress)
	{
		address = new TAddress;
		strcpy(address->ucn, ucn);
		address->ucnType = ucnType;

		if (!address->Try())	// unlikely
		{
			delete address;
			address = NULL;
		}

		triedAddress = true;
	}
}

// ----- TLegalSide ----------------------------------------------------------
TLegalSide::TLegalSide()
	: TSideWind("T_SIDE_WIND", TLegalSide::Clone), TTryAddress(NULL)
{
	*specifiedLawer = '\0';
}

TGroup *TLegalSide::Clone()
{
	return new TLegalSide;
}

void TLegalSide::Print(mstr &m)
{
	m.printf("%s - ", Involvement->Seek(involvement));
	TSideWind::PrintNames(m);
	m.cat(" - ");
	legalside_address(m, this);
	m.compress();
}

// ----- TDecision -----------------------------------------------------------
TDecision::TDecision(TGroup *(*const tClone)())
	: TIndexContainer("T_DECISION", tClone), surrouendoc(NULL), virtualAct(false)
{
	Add(
		new TDate("F_FROM_DATE", &fromDate, true),
		new TDate("F_RETURNED", &returned, true),
		new TCharAlias("F_RESULT", &result),
		new TText("F_TEXT", &text, SIZE_OF_DECISION_TEXT),
		new TDate("F_ENTER_DATE", &enterDate, false),
		new TCharAlias("F_SESSION_KIND", &sessionKind),
		new TLong("F_REGISTER", &registry),
		new TLong("F_VOLUME", &volume),
		new TLong("F_PAGE", &page),
		new TLong("F_PARTITION", &partition),
		new TLong("F_LEVEL", &level),
		new TCharList("F_ACCOMPLYS", accomplys, sizeof accomplys),
		new TText("F_SYNDICATE", &syndicate, SIZE_OF_SYNDICATE),
		new TLong("F_FLAGS", &flags),
		new TText("F_MOTIVES", &motives, SIZE_OF_MOTIVES),
		new TCheck("F_FLAGS_CONFESS", &flags, DECISION_CONFESS),
		new TCheck("F_FLAGS_ABSENCE", &flags, DECISION_ABSENCE),
		new TDate("F_FORCE_DATE", &forceDate, false),
		new TLong("F_EXEC_KIND", &execKind),
		new TMoney("F_DAMAGES", &damages),
		new TText("F_OPINION", &opinion, SIZE_OF_OPINION),
		new TCheck("F_FLAGS_PUBLIC", &flags, DECISION_PUBLIC),
		new TCharAlias("F_DEBTOR_STATUS", &debtorStatus),
		new TLongAlias("F_SLC_REASON", &slcReason),
		new TLong("F_FLAGS_X", &flagsX),
		new TCheck("F_FLAGS_CONTRACT", &flagsX, DECISION_X_CONTRACT),
		new TCheck("F_FLAGS_APPEAL", &flagsX, DECISION_X_APPEAL),
		new TCheck("F_FLAGS_PUBLTREG", &flagsX, DECISION_X_PUBLTREG),
		new TKeyWords("F_KEYWORDS", keyWords),
		new TDate("F_PUBLIC_DATE", &publicDate, false),
		NULL
	);

	descs = Descs + TABLE_DECISION;
}

TGroup *TDecision::Clone()
{
	return new TDecision;
}

#if COURT_TYPE == COURT_AREAL
const char *TDecision::Accomplys(const TLawsuit *lawsuit, const TSession *session) const
{
	if (strchr(KIND_INCONSISTENCE_INDOCS, lawsuit->sourceKind) && strchr(KIND_ACCOMPLYABLE_ENDOCS, kind))
		return session->Accomplys();

	return ACCOMPLY_NULLS;
}
#endif  // AREAL

sign TDecision::Appealed() const
{
	msql w;
	OutRegCriteria(w, "O.F_NO");

	TQuery q(str(w));
	return q.Read() ? q.Read() ? -1 : true : false;
}

bool TDecision::Combine(TRCDKeyContainer *source)
{
	if (PersonalData())
	{
		if (!TGroup::Insert())
			return false;

		Signal(ELECTRIC_ADD);	// 2010:204 LPR: was missing

		mstr involvements;
		TSideWind sideWind;
		TJudgement judgement;

		Involvements(involvements);
		judgement.Transfer(source, this, str(involvements), &sideWind);
	}
	else if (!TIndexContainer::Combine(source))
		return false;

	return true;
}

#if COURT_TYPE == COURT_ADMIN
// 2008:275 LPR: separated ADMIN to eliminate PUNISHMENT_ stubs
const char *TDecision::ExtraPunishments(TLawsuit *)
{
	return PUNISHMENT_NULLS;
}
#endif  // ADMIN

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
const char *TDecision::ExtraPunishments(TLawsuit *)
{
	return PUNISHMENT_EXTRAS;
}
#endif  // APPEAL || AREAL || MILITARY

#if COURT_TYPE == COURT_REGIONAL
const char *TDecision::ExtraPunishments(TLawsuit *lawsuit)
{
	return SeekSubject(lawsuit) &&
		strchr(Subject->sd[0].attrib, POST_EXTRAOTHER) ? PUNISHMENT_EXTRA_OTHERS : PUNISHMENT_EXTRAS;
}
#endif  // REGIONAL

int TDecision::GetPriority(const TSession *session, const char *priorityResults) const
{
#if COURT_TYPE == COURT_REGIONAL
	if (strchr(KIND_PROTECTIONS, kind))
		return -1;
#endif  // REGIONAL

	int priority = 0;

	if (strchr(priorityResults ? priorityResults : RESULT_CANCELAWS, result))
		priority |= 0x80;

	if (session->result == RESULT_PUBLIC_DECISION && public_decision(session->result, kind))
		priority |= 0x40;

	priority |= 0x20 - strindex(KIND_ENDOCS, kind);
	return priority;
}

void TDecision::Init(const TSurroundment *tSurrouendoc)
{
	surrouendoc = tSurrouendoc;
	key = surrouendoc->lawsuit;
	kind = surrouendoc->decisionKind;
	date = surrouendoc->date;
	fromDate = surrouendoc->decisionDate;
	sessionKind = surrouendoc->sessionKind;
	result = surrouendoc->result;
	eventNo = surrouendoc->eventNo;
	text.cat(surrouendoc->text);
	flags = surrouendoc->flags;
	flagsX = surrouendoc->flagsX;
	strcpy(accomplys, surrouendoc->accomplys);
}

char TDecision::IndexKind() const
{
#if COURT_TYPE == COURT_REGIONAL
	// 2013:177
	if (kind == KIND_IMMED_PROTECT)
		return KIND_PROTECTIVE;
#endif  // REGIONAL
	return kind2VisibleKind(kind);
}

char TDecision::IndexType() const
{
	return key.type;
}

bool TDecision::Insert()
{
	if (result == RESULT_FAKE && sessionKind != KIND_OTHER_SESSION)
		result = '\0';

	return TIndexContainer::Insert();
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
char TDecision::JudicialActType(TLawsuit *, TSession *)
{
	return JUDICIAL_ACT_STANDARD;
}
#endif  // APPEAL || ADMIN || MILITARY || REGIONAL

#if COURT_TYPE == COURT_AREAL
char TDecision::JudicialActType(TLawsuit *lawsuit, TSession *session)
{
	// 2009:072 LPR: brand-new
	if (Accomplys(lawsuit, session) != ACCOMPLY_NULLS)
		return JUDICIAL_ACT_TRADE_SYNDIC;

	if (kind == KIND_ORDER && session->result == RESULT_INCONSISTENCE)
		return JUDICIAL_ACT_TRADE_SYNDIC;

	if (strchr(KIND_FIRM_PSDLIZ_ENDOCS, kind) && strchr(RESULT_FIRM_PSEUDOLIZEDS, session->result))
		return JUDICIAL_ACT_FIRM_ACCEPT;

	return JUDICIAL_ACT_STANDARD;
}
#endif  // AREAL

CDate TDecision::MinReturned(bool exself) const
{
	msql m(Name, "MIN(F_RETURNED) AS F_RETURNED");
	CDate minReturned;

	m.Add(this, "F_NO, F_YEAR, F_TYPE, F_DATE");

	if (exself)
		m.AddChar("NOT F_KIND", kind);

	long returned = dataBase->GetLong(str(m), "F_RETURNED");

	if (returned)
		minReturned.SetValue(returned);

	return minReturned;
}

bool TDecision::Motivable()
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	return key.type == TYPE_PUNISHMENT_LAW && !strchr(KIND_UNMOTIVABLES, kind);
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_ADMIN
	return false;
#endif  // ADMIN
}

bool TDecision::Motivated()
{
	return any(motives) || ((flags & DECISION_MOTIVES) && !returned.Empty());
}

bool TDecision::CopyTo(const CDate &tDate)
{
	date = tDate;

	if (TGroup::Insert())
	{
		Signal(ELECTRIC_ADD);
		return true;
	}

	return false;
}

void TDecision::OutRegIndexes(mstr &m, const char *sep) const
{
	msql w;
	TOutReg outReg;

	OutRegCriteria(w, "DISTINCT O.F_VSS_INDEX");
	w.AddIsNull("O.F_VSS_INDEX", false);
	w.AddOrder("O.F_VSS_INDEX");

	TQuery q(str(w));
	while (q.Read())
	{
		outReg << q;
		m.sep(sep ? sep : ", ");
		m.cat(outReg.vssIndex);
	}
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TDecision::P64Check(mstr &m, const TGroupArray *judgements, char lawsuitKind)
{
	bool has_defendants = false;

	for (int i = 0; i < judgements->Count(); i++)
	{
		TJudgement *judgement = (TJudgement *) (*judgements)[i];
		sign personalData = maybe_personal_data(this, judgement->involvement);

		if (personalData)
		{
			const char *incomplete = NULL;

			if (personalData == true)
				has_defendants = true;

			if (personalData == true && judgement->judged == JUDGED_NOT_KNOWN)
				incomplete = "вписан резултат от съдебното производство";
			else if (personalData == true && judgement->punishment == PUNISHMENT_NOT_KNOWN)
				incomplete = "вписано наказание";
			else if (strchr(PUNISHMENT_P64_LCPS, judgement->punishment) && judgement->notifyDate.Empty())
				incomplete = "вписана дата на получаване на уведомление за предприемане на действия по изпълнение";

			if (incomplete)
			{
				m.printf("Няма %s за лицето ", incomplete);
				judgement->PrintNames(m);
				break;
			}
		}
	}

	if (!any(m) && !secondary(lawsuitKind) && !has_defendants)
		m.cat("Няма десни страни");
}

bool TDecision::P64Wrong(const TLawsuit *lawsuit, const TInReg *inReg, bool appealedCheck)
{
	TJudgement judgement;
	mstr m;

	key = judgement.key = lawsuit->key;
	kind = judgement.kind = inReg->decisionKind;
	date = judgement.date = inReg->decisionDate;

	if (Try() && P64Direct(appealedCheck))
	{
		TGArray<TJudgement> judgements;

		judgement.LoadFloat(&judgements);
		P64Check(m, &judgements, lawsuit->kind);
	}

	return any(m);
}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

bool TDecision::P64Connect(char lawsuitKind) const
{
	TConnect connect;

	return key.type == TYPE_PUNISHMENT_LAW && secondary(lawsuitKind) && date >= P64ActDate &&
		(!connect.TryBy(key, CONNECT_TYPE_LOWER_INSTANCE, "F_DECISION_KIND") ||
		strchr(KIND_P64_CONNECT_ENDOCS, connect.decisionKind));
}

bool TDecision::P64Direct(bool appealedCheck, char tKind) const
{
	if (date < P64ActDate || !strchr(KIND_P64_DIRECT_ENDOCS, tKind ? tKind : kind))
		return false;

	return !appealedCheck || !Appealed();
}

bool TDecision::PersonalData(const TSession *session) const
{
	if (!strchr(key.type == TYPE_PUNISHMENT_LAW ? POST_CRIME_ATTRIBS : POST_OTHER_ATTRIBS, POST_PERSONAL))
		return false;

	// 2017:304 former PersonalKind
	switch (kind)
	{
	#if COURT_TYPE == COURT_REGIONAL
		case KIND_EXECUTIVE :
		case KIND_PROTECTIVE : return false;
	#endif  // REGIONAL
	// 2017:304 note: JUDGEMENT and BULWARK remain undecided for AppC
	#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		case KIND_JUDGEMENT :
		case KIND_BULWARK : return true;
	#endif  // AREAL || MILITARY || REGIONAL
		case KIND_PROTOCOL : return false;
	}

	TDecision tDecision;
	TSession tSession;

	tDecision << this;
	// 2008:178 optimized: only get session kind
	if (!tDecision.sessionKind)
	{
		if (session)
			tDecision.sessionKind = session->kind;
		else if (!tDecision.Get("F_SESSION_KIND"))
			return false;
	}

	if (session)
		tSession << session;
	else
	{
		tSession.key = key;
		tSession.date = date;
		tSession.kind = tDecision.sessionKind;

		if (!tSession.Get("F_RESULT"))
			return false;
	}

	// 2017:304 former TSession::PersonalKind()
	if (kind == KIND_SETTLEMENT && !strchr(RESULT_PERSONAL_SETTLEMENTS, tSession.result))
		return false;

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (kind == KIND_ORDER && tSession.result != RESULT_WITH_FINAL_ORDER)
		return false;
#endif  // AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN
	if (kind == KIND_ORDER)
		return false;
#endif  // APPEAL || ADMIN

	return tSession.PersonalData();
}

bool TDecision::PreferMotives(const TLawsuit *lawsuit) const
{
#if TESTVER
	Subject->Seek(lawsuit->key.type, lawsuit->subject);
	bool x1 = strchr(KIND_PREFER_MOTIVES, kind) != NULL;
	bool x2 = strchr(Subject->Attrib(lawsuit->kind), POST_REVERT_DATE) != NULL;
	return x1 ^ x2;
#else  // TESTVER
	return (strchr(KIND_PREFER_MOTIVES, kind) != NULL) ^ (strchr(Subject->Attrib(lawsuit->kind), POST_REVERT_DATE) != NULL);
#endif
}

const char *TDecision::Punishments(TLawsuit *lawsuit)
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	(void) lawsuit;
#endif  // APPEAL || ADMIN || MILITARY || REGIONAL

	// 2009:211 TRQ: order ::= settlement
	if (kind == KIND_SETTLEMENT || kind == KIND_ORDER)
		return PUNISHMENT_NOT_KNOWNS;

	// 2008:275 LPR: separated ADMIN to eliminate PUNISHMENT_ stubs
#if COURT_TYPE == COURT_ADMIN
	fatal("%d: ADMIN: kind not supporteded by punishments", kind);
#endif  // ADMIN

#if COURT_TYPE == COURT_AREAL
	// 2016:095 LPR: sync w/ LocalJudgementDialog::Limited
	if (lawsuit->kind == KIND_PRIVATE_PUNISHMENT_LAW && strchr(KIND_DECISIONS, kind))
		return PUNISHMENT_NOT_KNOWNS;
#endif  // AREAL

	// 2017:307 disabled, no full 2nd personal data
	//if (kind == KIND_DECISION && secondary(lawsuit->kind) && lawsuit->PostPersonal())
	//	return PUNISHMENT_JUDGEMENTS;

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	switch (kind2VisibleKind(kind))
	{
		case KIND_DECISION : return PUNISHMENT_DECISIONS;
		case KIND_JUDGEMENT : return PUNISHMENT_JUDGEMENTS;
		case KIND_BULWARK : return PUNISHMENT_BULWARKS;
		default : error("%d: kind not supported by punishments", kind);
	}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

	return PUNISHMENT_NULLS;
}

struct TStandardResult
{
	char decisionKind;
	char decisionResult;
};

static const TStandardResult StandardResults[] =
{
	{ KIND_DECISION,		RESULT_READY_DECISION },
	{ KIND_ORDER,		RESULT_READY_ORDER },
	{ KIND_SETTLEMENT,	RESULT_READY_SETTLEMENT },
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ KIND_JUDGEMENT,		RESULT_READY_JUDGEMENT },
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ KIND_PROTOCOL,		RESULT_READY_PROTOCOL },
#endif  // ADMIN || AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ KIND_BULWARK,		RESULT_READY_BULWARK },
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
	{ KIND_PROTECTIVE,	RESULT_READY_EXEC_PROT },
#endif  // AREAL || REGIONAL
#if COURT_TYPE == COURT_REGIONAL
	{ KIND_EXECUTIVE,		RESULT_READY_EXEC_PROT },
	{ KIND_IMMED_PROTECT,	RESULT_READY_EXEC_PROT },
	{ KIND_EUHER_CERT,	RESULT_READY_CERTIFICATE },
#endif  // REGIONAL
	// 2012:144 comout
	//{ KIND_DECREE,		RESULT_READY_DECREE },
	{ '\0',			'\0' }
};

char TDecision::StandardResult(char kind)
{
	char tKind = kind2VisibleKind(kind);

	for (const TStandardResult *result = StandardResults; result->decisionKind; result++)
		if (result->decisionKind == tKind)
			return result->decisionResult;

	fatal("%d: decision kind not supported by standard result", tKind);
	return '\0';
}

struct TSpecificResult
{
	const char *requestKinds;
	const char *appealedKinds;
	const char *sessionResults;
	const char *decisionKinds;
	char subjectAttrib;
	const char *decisionResults;
};

// ----- KIND_ ---------------------------------------------------------------
// ----- RESULT_ -------------------------------------------------------------
// ----- POST_ ---------------------------------------------------------------
static const TSpecificResult SpecificResults[] =
{
	// 2012:004 LRQ: appealedKinds +'-' where л
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ NULL, NULL, RESULT_BULWARKABLES, KIND_SETTLEMENTS, '\0', "T" },		// 2011:234
#endif  // AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	{ NULL, NULL, RESULT_ALL_CANCELEDS, "лмШ", '\0', "Ш" },	// 2017:157 +Ш
#endif  // APPEAL || ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
	{ NULL, NULL, RESULT_ALL_CANCELEDS, "SмE", '\0', "Ш" },	// 2011:231 FIX: was лм; 2017:157 +E
#endif  // REGIONAL

#if COURT_TYPE == COURT_APPEAL
	// 2016:236 -R, does not exist
	{ KIND_SLOW_REQUESTS,	NULL,			RESULT_COMPLETIZEDS,	"Шлм",	'\0', "Я12uШ" },
	{ KIND_RENEW_INDOCS,	NULL,			"p",				"Ш",		'K',  "Яqu" },
	{ KIND_CT274_INDOCS,	NULL,			"p",				"Ш",		'C',  "ЯvwzШ" },
	{ NULL,			NULL,			"п",				"л",		'\0', "qТ" },
	{ NULL,			NULL,			"A",				"л",		'\0', "тТ" },
	{ NULL,			NULL,			"о",				"л",		'\0', "с" },
	{ NULL,			NULL,			"мр",				"Ш",		'\0', "y" },
	{ NULL,			KIND_JUDGEMENTS,	RESULT_COMPLETIZEDS,	"ъШ",		'\0', "ГДИЙЛПУФЦЇЧ" },
	{ NULL,			KIND_JUDGEMENTS,	RESULT_COMPLETIZEDS,	"лм",		'\0', "ГДИЙЛПУФЦЇЧШ" },
	{ NULL,			KIND_DECISIONS,	RESULT_COMPLETIZEDS,	"Шлм",	'\0', "CEGїѕHКШ" },
	{ NULL,			"лм-",		RESULT_COMPLETIZEDS,	"Шлм",	'\0', "vwzОШКuс" },	// 2012:137 +uс
#endif  // COURT_APPEAL

#if COURT_TYPE == COURT_ADMIN
	{ NULL,			KIND_DECISIONS,	RESULT_COMPLETIZEDS,	"ШRлм",	'\0',	"CEGїѕHКШ" },	// Ґ if APC
	{ NULL,			"лм-",		RESULT_COMPLETIZEDS,	"ШRлм",	'\0',	"vwzОШК" },
	{ NULL,			KIND_NULL,		RESULT_COMPLETIZEDS,	"л",		'\0', "TШ" },
	// 2012:094 LPR: for compatibility with existing endoc results
	{ NULL,			NULL,			NULL,				"",		'\0', "quЯҐМ78b" },
#endif  // ADMIN

#if COURT_TYPE == COURT_AREAL
	{ KIND_FIRM_ALLDOCS,	NULL,			"FKЁё",			"ШRлм",	'\0', "яьгжщъюйлпфцчш123моЈ" },
	{ KIND_FIRM_ALLDOCS,	NULL,			"a",				"лм",		'\0', "Ј" },
	{ KIND_SLOW_REQUESTS,	NULL,			RESULT_COMPLETIZEDS,	"ШRлм",	'\0', "Я78uШ" },
	// 2017:066 +р after ЙЛП
	{ NULL,			KIND_JUDGEMENTS,	RESULT_COMPLETIZEDS,	"ъ",		'\0', "ГДИЙЛПрУФЦЇЧ" },	// 2011:314 -ШR
	{ NULL,			KIND_JUDGEMENTS,	RESULT_COMPLETIZEDS,	"лмШR",	'\0', "ГДИЙЛПрУФЦЇЧШ" },// 2011:314 +ШR
	{ NULL,			KIND_DECISIONS,	RESULT_COMPLETIZEDS,	"ШRлм",	'\0', "CEGїѕHКШ" },
	{ NULL,			"лмЫE-",		RESULT_COMPLETIZEDS,	"ШRлм",	'\0', "vwzОШК" },
	{ NULL,			NULL,			RESULT_COMPLETIZEDS,	"лм",		'C',  "ЯvwzШ" },
	{ NULL,			NULL,			RESULT_COMPLETIZEDS,	"лм",		'K',  "Яqu" },
	{ NULL,			NULL,			RESULT_COMPLETIZEDS,	"л",		'\0', "TШ" },
	// 2012:094 LPR: for compatibility with existing endoc results
	{ NULL,			NULL,			NULL,				"",		'\0', "ҐМ" },
#endif  // AREAL

#if COURT_TYPE == COURT_MILITARY
	{ NULL,			KIND_JUDGEMENTS,	RESULT_COMPLETIZEDS,	"ъШR",	'\0', "ГДИЙЛПУФЦЇЧ" },
	{ NULL,			KIND_JUDGEMENTS,	RESULT_COMPLETIZEDS,	"лм",		'\0', "ГДИЙЛПУФЦЇЧШ" },
	{ NULL,			KIND_DECISIONS,	RESULT_COMPLETIZEDS,	"ШRлм",	'\0', "CEGїѕHКШ" },
	{ NULL,			"лм-",		RESULT_COMPLETIZEDS,	"ШRлм",	'\0', "vwzОШК" },
	{ NULL,			NULL,			RESULT_COMPLETIZEDS,	"лм",		'C',  "ЯvwzШ" },
	{ NULL,			NULL,			RESULT_COMPLETIZEDS,	"лм",		'K',  "Яqu" },
	{ NULL,			NULL,			RESULT_COMPLETIZEDS,	"л",		'\0', "TШ" },
	// 2012:094 LPR: for compatibility with existing endoc results
	{ NULL,			NULL,			NULL,				"",		'\0', "ҐМ78" },
#endif  // MILITARY

#if COURT_TYPE == COURT_REGIONAL
	// 2016:236 +П where E
	{ KIND_PUNISH_DECREES,		NULL,			NULL,                   "EП",		'\0', "txX" },	// 2011:208 +u
	{ C2S[KIND_REQUEST_126G],	NULL,			"d",				"Sм",		'\0', "GH" },
	// 2014:302 HERITAGE-s
	{ C2S[KIND_HERITAGE_ACCEPT],	NULL,			NULL,				"S",		NULL, "ЁШ" },
	{ C2S[KIND_HERITAGE_REJECT],	NULL,			NULL,				"S",		NULL, "ёШ" },
	{ C2S[KIND_HERITAGE_REQ_51],	NULL,			NULL,				"S",		NULL, "ЁёШ" },
	{ C2S[KIND_HERITAGE_ACCEPT],	NULL,			NULL,				"EПм",	NULL, "Ё" },
	{ C2S[KIND_HERITAGE_REJECT],	NULL,			NULL,				"EПм",	NULL, "ё" },
	{ C2S[KIND_HERITAGE_REQ_51],	NULL,			NULL,				"EПм",	NULL, "Ёё" },
	{ NULL,				NULL,			NULL,				"‚",		NULL, "Ђ" },	// 2017:207 p.39
	{ NULL,				NULL,			NULL,				"Sм",		'C',  "ЯvwzШ" },
	{ NULL,				NULL,			NULL,				"Sм",		'K',  "Яqu" },
	{ NULL,				NULL,			NULL,				")",		'\0', "є" },
	{ NULL,				NULL,			RESULT_COMPLETIZEDS,	"S",		'\0', "TШ" },
#endif  // REGIONAL
	{ NULL, NULL, NULL, NULL, '\0', "" }
};

const char *TDecision::SpecificResults(TLawsuit *lawsuit, TSession *session, TDecResData *data)
{
	const TSpecificResult *result;

	for (result = ::SpecificResults; result->decisionKinds; result++)
	{
		if (!strmatch(lawsuit->sourceKind, result->requestKinds))
			continue;

		if (!strmatch(session->result, result->sessionResults) || !strchr(result->decisionKinds, kind))
			continue;

		if (result->appealedKinds)
		{
			if (!data->appealedKindLoaded)
			{
				TConnect connect;
				data->appealedKind = connect.TryLower(this, "F_DECISION_KIND") ? connect.decisionKind : '\0';
				data->appealedKindLoaded = true;
			}

			if (!strmatch(data->appealedKind, result->appealedKinds))
				continue;
		}

		if (result->subjectAttrib)
		{
			if (!data->subjectAttribLoaded)
			{
				strcpy(data->subjectAttrib, SeekSubject(lawsuit) ? Subject->Attrib(lawsuit->kind) : "");
				data->subjectAttribLoaded = true;
			}

			if (!strmatch(result->subjectAttrib, data->subjectAttrib))
				continue;
		}

		break;
	}

	return result->decisionResults;
}

bool TDecision::SeekSubject(TLawsuit *lawsuit)
{
	*Subject << lawsuit;
	return Subject->Get();
}

bool TDecision::Update(const char *fields)
{
	if (result == RESULT_FAKE && sessionKind != KIND_OTHER_SESSION)
		result = '\0';

	return TIndexContainer::Update(fields);
}

void CreateEndocResults()
{
	*_RESULT_WITH_READY_ANYS = '\0';	// 2012:124 auto-compute

	for (const TStandardResult *result = StandardResults; result->decisionKind; result++)
		strcach_nx(_RESULT_WITH_READY_ANYS, result->decisionResult);

	strcpy(_RESULT_ENDOCS, _RESULT_WITH_READY_ANYS);

	for (const TSpecificResult *result = SpecificResults; result->decisionKinds; result++)
		strcat_nx(_RESULT_ENDOCS, result->decisionResults);

#if COURT_TYPE == COURT_APPEAL
	strcach(_RESULT_ENDOCS, RESULT_ACT_STANDING);	// for compatibility
#endif  // APPEAL
}

// ----- TDecisionRider ------------------------------------------------------
TDecisionRider::TDecisionRider(TDecision *tDecision, char tKind)
	: decision(tDecision)
{
	*this << tDecision;

	if ((kind = tKind) == KIND_MOTIVES)
		fromDate = returned;
}

// ----- TJudgement ----------------------------------------------------------
TJudgement::TJudgement()
	: TSideWind("T_JUDGEMENT", TJudgement::Clone)
{
	Add(
		new TCharAlias("F_JUDGED", &judged),
		new TCharAlias("F_PUNISHMENT", &punishment),
		new TCharAlias("F_RECIDIVE", &recidive),
		new TText("F_TEXT", &text, SIZE_OF_JUDGEMENT_TEXT),
		new TDate("F_FORCE_DATE", &forceDate, false),
		new TDate("F_SEND_DATE", &sendDate, false),
		new TString("F_SEND_TO", sendTo, sizeof sendTo),
		new TDate("F_EXEC_DATE", &execDate, false),
		new TString("F_EXEC_BIND", execBind, sizeof execBind),
		new TString("F_EXEC_PLACE", execPlace, sizeof execPlace),
		new TLong("F_FLAGS_X", &flagsX),
		new TCheck("F_FLAGS_ADDITIONAL", &flagsX, JUDGEMENT_X_ADDITIONAL),
		new TCheck("F_FLAGS_NEW", &flagsX, JUDGEMENT_X_NEW),
		new TCheck("F_FLAGS_PRIORITY", &flagsX, JUDGEMENT_X_PRIORITY),
		new TString("F_FORGIVE", forgive, sizeof forgive),
		new TString("F_ORDER_FROM", orderFrom, sizeof orderFrom),
		new TDate("F_GIVEN_DATE", &givenDate, false),
		new TString("F_SEND_TEXT", sendText, sizeof sendText),
		new TDate("F_COUNT_DATE", &countDate, false),
		new TCharAlias("F_INTERVAL", &interval),
		new TString("F_EXEC_REMARK", execRemark, sizeof execRemark),
		new TText("F_EXEC_TEXT", &execText, SIZE_OF_JUDGEMENT_TEXT),
		new TLong("F_FLAGS_Y", &flagsY),
		new TCheck("F_FLAGS_APPENDIX_23", &flagsY, JUDGEMENT_Y_APPENDIX_23),	// 2011:019
		new TCheck("F_FLAGS_APPENDIX_24", &flagsY, JUDGEMENT_Y_APPENDIX_24),
		new TCheck("F_FLAGS_APPENDIX_25", &flagsY, JUDGEMENT_Y_APPENDIX_25),
		new TCheck("F_FLAGS_APPENDIX_26", &flagsY, JUDGEMENT_Y_APPENDIX_26),
		new TCharList("F_PUNISHMENTS_X", punishmentsExtra, sizeof punishmentsExtra),
		new TCharAlias("F_JAIL_MODE", &jailMode),
		new TLongAlias("F_ARTICLE", &article),
		new TDate("F_NOTIFY_DATE", &notifyDate, false),
		NULL
	);

	judged = JUDGED_NOT_KNOWN;
	punishment = PUNISHMENT_NOT_KNOWN;
	recidive = RECIDIVE_NOT_KNOWN;
	interval = INTERVAL_NONE_YET;
	jailMode = JAIL_MODE_NONE_YET;
}

TGroup *TJudgement::Clone()
{
	return new TJudgement;
}

void TJudgement::Print(mstr &m)
{
	mstr tJudged;
	size_t pos = m.len();

	TSideWind::Print(m);
	tJudged.cat(Judged->Seek(judged));
	tJudged.setlen(SIZE_OF_JUDGEMENT_STAT);
	mbk_insert(&m, str(tJudged), pos + SIZE_OF_INVOLVEMENT_DISPLAY, SIZE_OF_JUDGEMENT_STAT);
}

void TJudgement::PrintState(mstr &m)
{
	m.printf("%c %c", ChildText()[0], side_wind_notified_mark(this, eventStatus));
}

char TJudgement::Status()
{
	return judged;
}

// ----- TSurroundment -------------------------------------------------------
TSurroundment::TSurroundment()
	: TIndexContainer("T_SURROUNDMENT", TSurroundment::Clone), sessionDate(date)
{
	Add(
		NEW_RCDKEY("F_LAWSUIT_", &lawsuit),
		new TCharAlias("F_SESSION_KIND", &sessionKind),
		new TCharAlias("F_RESULT", &result),
		new TText("F_TEXT", &text, SIZE_OF_SURROUNDMENT_TEXT),
		new TCharAlias("F_DECISION_KIND", &decisionKind),
		new TDate("F_DECISION_DATE", &decisionDate, false),	// 2016:174 FIX: false
		new TLong("F_EXTRA_NO", &extraNo),
		new TCharList("F_ACCOMPLYS", accomplys, sizeof accomplys),
		new TCharAlias("F_DEBTOR_STATUS", &debtorStatus),
		new TLong("F_FLAGS", &flags),
		new TCheck("F_FLAGS_PUBLIC", &flags, DECISION_PUBLIC),
		new TLong("F_FLAGS_X", &flagsX),
		new TCheck("F_FLAGS_PUBLTREG", &flagsX, DECISION_X_PUBLTREG),
		NULL
	);

	descs = Descs + TABLE_SURROUNDMENT;
	key.type = TYPE_SURROUND;
}

TGroup *TSurroundment::Clone()
{
	return new TSurroundment;
}

void TSurroundment::BasicCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_NO, F_YEAR, F_TYPE, F_KIND");
}

bool TSurroundment::ClearSurrouendoc()
{
	if (DeleteIndex())
	{
		result = RESULT_SURROUNDED;
		text.clear();
		decisionKind = '\0';
		decisionDate.Clear();
		eventNo = 0;
		extraNo = 0;
		*accomplys = '\0';
		debtorStatus = '\0';
		flags = flagsX = 0;
		return Update();
	}

	return false;
}

const char *TSurroundment::SurrouendocResults()
{
	return decisionKind == KIND_DECISION ? RESULT_SURROUENDOC_DECISIONS : RESULT_SURROUENDOC_SETTLEMENTS;
}

bool TSurroundment::Delete(bool exact)
{
	bool deleted = TGroup::Delete(exact);

	if (exact && deleted)
		DeleteIndex();

	return deleted;
}

void TSurroundment::ExactCriteria(msql &m, const char *fields)
{
	BasicCriteria(m, fields);
	SessionCriteria(m);
}

void TSurroundment::FloatCriteria(msql &m, const char *fields)
{
	BasicCriteria(m, fields);
	m.AddOrder("F_DATE");
}

void TSurroundment::Print(mstr &m)
{
	m.printf("%-40s  %5d/%4d  %s", Kind->Seek(kind), key.no, key.year, Result->Seek(result));
}

void TSurroundment::Init(const TSurround *surround, const TSession *session)
{
	key = surround->key;
	kind = surround->kind;
	lawsuit = surround->lawsuit;
	sessionKind = session->kind;
	sessionDate = session->date;
	result = RESULT_SURROUNDED;
	text.cpy("");
}

void TSurroundment::SessionCriteria(msql &m)
{
	m.Add(this, "F_LAWSUIT_NO, F_LAWSUIT_YEAR, F_LAWSUIT_TYPE, F_SESSION_KIND, F_DATE");
}

void TSurroundment::SessionCriteria(msql &m, const TRCDKey &key, char kind, const CDate &date)
{
	lawsuit = key;
	sessionKind = kind;
	sessionDate = date;
	SessionCriteria(m);
}

void TSurroundment::SessionCriteria(msql &m, const TSession *session)
{
	SessionCriteria(m, session->key, session->kind, session->date);
}

char TSurroundment::IndexKind() const
{
	return decisionKind;
}

char TSurroundment::IndexType() const
{
	return decisionKind ? lawsuit.type : '\0';
}

bool TSurroundment::Insert()
{
	if (IndexEvent())
	{
		if (TGroup::Insert())
			return true;

		if (DecisionIndex->type)
			DecisionIndex->Delete(true);
	}

	return false;
}

bool TSurroundment::Update(const char *fields)
{
	return TRCDKeyGroup::Update(fields);
}

// ----- TSurround -----------------------------------------------------------
TSurround::TSurround()
	: TPrimeContainer("T_SURROUND", TSurround::Clone), xfer(false)
{
	Add(
		new TCharAlias("F_COLLEGE", &college),
		new TString("F_TEXT", text, sizeof text),
		new TPersonAlias("F_JUDGE", judge, sizeof judge),
		new TTime("F_TIME", &time),
		new TCharAlias("F_DECISION_KIND", &decisionKind),
		new TDate("F_DECISION_DATE", &decisionDate, false),
		new TString("F_DIRECTION", direction, sizeof direction),
		NULL
	);

	descs = Descs + TABLE_SURROUND;
	key.type = TYPE_SURROUND;
	kind = '\0';
	antiNoRight = RIGHT_ANTIDOCNO;
}

TGroup *TSurround::Clone()
{
	return new TSurround;
}

void TSurround::ExactCriteria(msql &m, const char *fields)
{
	TRCDKeyContainer::ExactCriteria(m, fields);
}

void TSurround::FloatCriteria(msql &m, const char *fields)
{
	TPrimeContainer::FloatCriteria(m, fields);
	m.AddOrder("F_YEAR, F_NO");	// 2007:319 LRQ
}

bool TSurround::Generate()
{
	return TRCDKeyContainer::Generate();
}

bool TSurround::Latest(TRCDKeyContainer *container)
{
	if (xfer)
		return false;

	return TPrimeContainer::Latest(container);
}

bool TSurround::Insert()
{
	key.year = date.year;

	if (time.Empty())
		time = LNow();

	*RequestIndex << this;
	if (!RequestIndex->Insert())
		return false;
	*this << RequestIndex;

#if COURT_TYPE == COURT_AREAL
	// 2007:240 LRQ/LPR: FIX: for FIRM CHANGE_SURROUNDS only
	if (lawsuit.type == TYPE_FIRM_LAW && kind == KIND_CHANGE_SURROUND && !atob(judge))
	{
		// *judge = '\0';
		strcpy(judge, INTER);
	}
#endif  // AREAL		

	if (!TDatedContainer::Insert())
	{
		RequestIndex->Delete(true);
		return false;
	}

	return true;
}

void TSurround::Involvements(mstr &m) const
{
#if 20170228
	TDatedContainer container("T_CONTAINER", NULL);

	container.kind = kind;
	container.key = lawsuit;
	container.Involvements(m);
#else  // 20170228
	TDatedContainer::Involvements(s);

	TRCDKey tKey = key;
	key = lawsuit;
	TDatedContainer::Involvements(s + strlen(s));
	key = tKey;
#endif  // 20170228
}

TSurroundment *TSurround::LastSurroundment(const char *fields) const
{
	TSurroundment *surroundment = new TSurroundment;
	msql m(surroundment->Name, fields);

	m.Add(this, "F_NO, F_YEAR, F_TYPE");
	m.AddOrder("F_DATE DESC");
	TQuery q(str(m));

	if (q.Read())
		*surroundment << q;
	else
	{
		delete surroundment;
		surroundment = NULL;
	}

	return surroundment;
}

char TSurround::Status() const
{
	TSurroundment *const surroundment = LastSurroundment("F_RESULT");
	TVirtPtr VP(surroundment);
	return surroundment ? surroundment->Status() : TDatedContainer::Status();
}

bool TSurround::Update(const char *fields)
{
#if COURT_TYPE == COURT_AREAL
	// 2018-09-14: FIX: sync w/ insert; 2018-09-17: FIX: full sync
	if (lawsuit.type == TYPE_FIRM_LAW && kind == KIND_CHANGE_SURROUND && !atob(judge))
	{
		// *judge = '\0';
		strcpy(judge, INTER);
	}
#endif  // AREAL

	return TDatedContainer::Update(fields);
}

// ----- TLog ----------------------------------------------------------------
TLog::TLog(TGroup *(*const tClone)())
	: TGroup("T_LOG", tClone)
{
	Add(
		new TLong("F_LOG", &log),
		new TString("F_UCN", ucn, sizeof ucn),
		new TCharAlias("F_GOP", &gop),
		new TLong("F_OPC", &opc),
		new TTimeStamp("F_STAMP", &stamp),
		NEW_RCDKEY("F_", &key),
		new TChar("F_KIND", &kind),
		new TDate("F_DATE", &date, false),
		new TString("F_TEXT", text, sizeof text),
		new TLong("F_RES_1", resBlock + 2),
		new TLong("F_RES_2", resBlock + 1),
		new TLong("F_RES_3", resBlock + 3),
		new TLong("F_RES_4", resBlock + 0),
		NULL
	);

	descs = Descs + TABLE_LOG;
}

TGroup *TLog::Clone()
{
	return new TLog;
}

void TLog::CalcMD5(long *tResBlock)
{
	char buffer[128];
	char *ptr = buffer + 8;
	char *s;

	for (s = buffer; s < buffer + sizeof buffer; s += sizeof(int))
		*(int *) s = rand();

	long pKey[2] = { key.no, (key.year << 11) + key.type };
	memcpy(ptr, pKey, sizeof pKey);
	ptr += sizeof pKey;

	char pStamp[6] = { (char) (stamp.year - YEAR_MIN), (char) stamp.month, (char) stamp.day, (char) stamp.hour,
		(char) stamp.minute, (char) stamp.second };
	memcpy(ptr, pStamp, sizeof pStamp);
	ptr += sizeof pStamp;

	*(int *) ptr = atoi(ucn);
	ptr += sizeof(int);

	memcpy(ptr, &gop, sizeof gop);
	ptr += sizeof gop;

	memcpy(ptr, &log, sizeof log);
	ptr += sizeof log;

	memcpy(ptr, &opc, sizeof opc);
	ptr += sizeof opc;

	char pDate[3] = { (char) (date.year - YEAR_MIN), (char) date.month, (char) date.year };
	memcpy(ptr, pDate, sizeof pDate);
	ptr += sizeof pDate;

	strcpy(ptr, text);
	ptr += strlen(text);

	unsigned seed = 0;

	for (s = buffer + 8; s < ptr; s++)
		seed ^= *s << (8 * ((s - buffer) & 0x03));

	srand(seed);
	*(int *) buffer = rand();
	*(int *) (buffer + 4) = rand();

	while (s < buffer + sizeof buffer)
		*s++ = (char) (rand() & 0xFF);

	calc_md5sum(buffer, sizeof buffer, (char *) tResBlock);
}

void TLog::CheckMD5()
{
	long tResBlock[4];

	CalcMD5(tResBlock);
	if (memcmp(tResBlock, resBlock, sizeof resBlock))
		fatal("%s: невалидна контролна сума.", Name);
}

void TLog::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.AddLong("F_LOG", log);
}

void TLog::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.AddOrder("F_STAMP");
}

bool TLog::Insert()
{
	strimall(text);
	CalcMD5(resBlock);
	return TGroup::Insert();
}

bool TLog::IsAnnounce() const
{
	long court, announce;

	return opc == CM_DELIV_EXTERNAL &&
		sscanf(text, LOG_ANNOUNCE_FORMAT, &court, &announce) == 2 &&
		court > 0 && announce > 0;
}

bool TLog::TypeName(mstr &m) const
{
	if (key.type)
	{
		TCharAliasGroup *alias;

		if (IsAnnounce())
		{
			ConnectKind->connectKind = type2ConnectKind(key.type);
			alias = ConnectKind;
		}
		else
		{
			Type->type = key.type;
			alias = Type;
		}

		if (alias->Try("F_NAME"))
		{
			m.cat(alias->NameField->S());
			return true;
		}

		printf("%d", key.type);
	}

	return false;
}

void TLog::Print(mstr &m)
{
	m.printf("%0D %-lT ", &((CDate &) stamp), &((CTime &) stamp));

	GOP->gop = gop;

	if (GOP->Try())
		m.printf("%-15s ", GOP->name);
	else
		m.printf("%-15d ", gop);

	// 2018-07-04: LPR
	size_t len = m.len();

	if (TypeName(m))
		m.cut(len + 1);
	else if (m.len() == len)
		m.cat(" ");

	m.cat(" ");
	//m.printf("%c ", key.type ? *Type->Seek(key.type) : ' ');

	if (key.no)
		m.printf("%5ld ", key.no);
	else
		m.printf("%-5s ", "");

	if (key.year)
		m.printf("%4ld ", key.year);
	else
		m.printf("%-4s ", "");

	// 2011:091 LRQ: date support
	len = m.len();

	if (any(text))
		m.cat(text);
	else if (kind)
		m.cat(Kind->Seek(kind));

	date.Print(m, " %D");
	m.setlen(len + SIZE_OF_LOG_TEXT + LENGTH_OF_DATE);
	len += SIZE_OF_LOG_TEXT + LENGTH_OF_DATE;

	m.cat(" ");
	print_user(m, ucn, UCN_CITIZEN_UCN, false);
	m.cut(len + 35);
}

bool TLog::ResBlock()
{
	return (resBlock[0] | resBlock[1] | resBlock[2] | resBlock[3]) != 0;
}

bool TLog::Update(const char *fields)
{
	strimall(text);
	CalcMD5(resBlock);
	return TGroup::Update(fields);
}

// ----- TIOReg --------------------------------------------------------------
TIOReg::TIOReg(const char *name, TGroup *(*const tClone)())
	: TPrimeContainer(name, tClone)
{
	Add(
		new TCharAlias("F_COLLEGE", &college),
		new TStringAlias("F_RECEIVER", receiver, sizeof receiver),
		new TCharAlias("F_RECEIVER_TYPE", &receiverType),
		NULL
	);

	receiverType = UCN_INSTANCE_RESERVED;
}

void TIOReg::ExactCriteria(msql &m, const char *fields)
{
	TRCDKeyContainer::ExactCriteria(m, fields);
}

bool TIOReg::Generate()
{
	return TRCDKeyContainer::Generate();
}

bool TIOReg::Insert()
{
	key.year = date.year;
	key.type = kind2type(kind);
	return TRCDKeyContainer::Insert();
}

// ----- TOutReg -------------------------------------------------------------
TOutReg::TOutReg()
	: TIOReg("T_OUTREG", TOutReg::Clone), xfer(NULL), aret(false), resend(false)
{
	Add(
		new TLong("F_IN_NO", &inNo),
		new TCharAlias("F_RESULT", &result),
		new TText("F_REMARK", &remark, SIZE_OF_OUTREG_REMARK),
		new TText("F_REMARK_VZ", &remarkVZ, SIZE_OF_OUTREG_REMARK),
		new TText("F_REMARK_KS", &remarkKS, SIZE_OF_OUTREG_REMARK),
		new TDate("F_RETURNED", &returned, false),
		new TLong("F_IN_YEAR", &inYear),
		new TCharAlias("F_RESULT_I", &resultI),
		new TCharAlias("F_RESULT_II", &resultII),
		new TCharAlias("F_RESULT_KS", &resultKS),
		new TCharAlias("F_RESULT_MJ", &resultMJ),
		new TLong("F_FLAGS", &flags),
		new TRadio("F_FLAGS_NONE", &flags, OUTREG_NONE),
		new TRadio("F_FLAGS_REGULAR", &flags, OUTREG_REGULAR),
		new TRadio("F_FLAGS_R", &flags, OUTREG_R),
		new TRadio("F_FLAGS_AR", &flags, OUTREG_AR),
		new TRadio("F_FLAGS_PACKAGE", &flags, OUTREG_PACKAGE),
		new TStringAlias("F_VSS_INDEX", vssIndex, sizeof vssIndex),
		new TLong("F_FLAGS_X", &flagsX),
		new TCheck("F_FLAGS_COURIER", &flagsX, OUTREG_X_COURIER),
		new TCheck("F_FLAGS_FAX", &flagsX, OUTREG_X_FAX),
		new TCheck("F_FLAGS_EMAIL", &flagsX, OUTREG_X_EMAIL),
		new TCharAlias("F_DECISION_KIND", &decisionKind),
		new TDate("F_DECISION_DATE", &decisionDate, false),
		new TCheck("F_FLAGS_RESTART", &flagsX, OUTREG_X_RESTART),
		new TText("F_TEXT", &text, SIZE_OF_OUTREG_REMARK),
		new TCheck("F_DELIVERY_NOTICE", &deliveryNotice, TRUE),
		NULL
	);

	descs = Descs + TABLE_OUTREG;
	key.type = TYPE_OUTREG;
	kind = *KIND_OUT_REG_DOCS;
	result = RESULT_IOREG_SEND;
	resultI = resultII = RESOLUTION_FAKE;
	resultKS = resultMJ = RESOLUTION_FAKE;
	antiNoRight = RIGHT_ANTIDOCNO;
}

TGroup *TOutReg::Clone()
{
	return new TOutReg;
}

bool TOutReg::EPortalSendable() const
{
	return !strchr(UCN_NOT_EPORTALS, receiverType);
}

bool TOutReg::TryLower(TConnect &connect) const
{
	msql m(connect.Name);

	connect.key = lawsuit;
	strcpy(connect.ucn, receiver);
	connect.ucnType = receiverType;
	m.Add(&connect, "F_NO, F_YEAR, F_TYPE, F_UCN, F_UCN_TYPE");
	m.AddChars("F_CONNECT_TYPE", CONNECT_TYPE_LOWER_INSTANCES);

	TQuery q(str(m));
	if (!q.Read())
		return false;

	connect << q;
	return true;
}

void TOutReg::Involvements(mstr &m) const
{
	m.cach(INVOLVEMENT_RECEIVER);
}

bool TOutReg::Latest(TRCDKeyContainer *)
{
	return false;
}

void TOutReg::LoadInRegs(TGArray<TInRegEvent> *events, const char *eventFields, const char *inRegFields)
{
	TInRegEvent inRegEvent;
	TInReg inReg;

	inRegEvent.OutCriteria(this);
	inRegEvent.LoadFloat(events, eventFields);

	if (inRegFields)
	{
		msql m;

		for (int i = 0; i < events->Count(); i++)
		{
			TInRegEvent *tInRegEvent = (*events)[i];

			inReg << tInRegEvent;
			// keep events fields
			inReg.ExactCriteria(m, inRegFields);
			TQuery q(str(m));
			if (q.Read())
				*tInRegEvent << q;
		}
	}
}

bool TOutReg::operator<=(const TOutReg &that) const
{
	return lawsuit != that.lawsuit ? lawsuit < that.lawsuit : date <= that.date;
}

// ----- TInRegEvent ---------------------------------------------------------
TInRegEvent::TInRegEvent()
	: TIndexContainer("T_INREG_EVENT", TInRegEvent::Clone)
{
	Add(
		new TCharAlias("F_RESULT", &result),
		new TText("F_TEXT", &text, SIZE_OF_INREG_EVENT_TEXT),
		new TLong("F_OUT_NO", &outNo),
		new TLong("F_OUT_YEAR", &outYear),
		new TPersonAlias("F_JUDGE", judge, sizeof judge),
		new TDate("F_RETURNED", &returned, false),
		NULL
	);

	descs = Descs + TABLE_INREG_EVENT;
	key.type = TYPE_INREG;
	kind = *KIND_INREG_EVENTS;
}

TGroup *TInRegEvent::Clone()
{
	return new TInRegEvent;
}

void TInRegEvent::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_NO, F_YEAR, F_KIND, F_DATE");
}

void TInRegEvent::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);

	if (outNo)
	{
		m.Add(this, "F_OUT_NO, F_OUT_YEAR, F_DATE");
		m.AddChar("F_RESULT", Status());
	}
	else if (key.no)
	{
		m.Add(this, "F_NO, F_YEAR, F_TYPE");
		m.AddOrder("F_DATE, F_KIND DESC");	// 2011:019 +F_KIND
	}
	else
		fatal("%s: no data for FloatCriteria", Name);
}

char TInRegEvent::IndexKind() const
{
	return result == RESULT_RESO_SETTLEMENT ? KIND_SETTLEMENT :
		result == RESULT_RESO_STANDPOINT ? KIND_STANDPOINT : KIND_ORDER;
}

char TInRegEvent::IndexType() const
{
	char type = TYPE_NULL;

	if (result != RESULT_IOREG_SEND && !strchr(RESULT_APPEAL_RETURNEDS, result))
	{
		char college = find_college(this);

		if (college && strchr(TYPE_LAWSUITS, college))
			type = college;
	}

	return type;
}

void TInRegEvent::OutCriteria(const TOutReg *outReg)
{
	outNo = outReg->key.no;
	outYear = outReg->key.year;
	// 2009:194 FIX: outReg->date, RESULT_IOREG_SEND -> Status() ? date | returned
	date = outReg->Status() == RESULT_IOREG_SEND ? outReg->date : outReg->returned;
	result = outReg->Status();
}

void TInRegEvent::Print(mstr &m)
{
	TInReg inReg;
	inReg.key = key;
	m.printf("%-40s  %5ld  %4ld", Kind->Seek(inReg.Try("F_KIND") ? inReg.kind : kind), key.no, key.year);
}

// ----- TInReg --------------------------------------------------------------
TInReg::TInReg()
	: TIOReg("T_INREG", TInReg::Clone)
{
	Add(
		NEW_RCDKEY("F_DECISION_", &decision),
		new TCharAlias("F_DECISION_KIND", &decisionKind),
		new TDate("F_DECISION_DATE", &decisionDate, false),
		new TPersonAlias("F_JUDGE", judge, sizeof judge),
		new TCharAlias("F_SESSION_KIND", &sessionKind),
		new TString("F_DIRECTION", direction, sizeof direction),
		new TCharAlias("F_SURROUND_KIND", &surroundKind),
		new TLong("F_SURROUND_NO", &surroundNo),
		new TLong("F_SURROUND_YEAR", &surroundYear),
		new TTime("F_TIME", &time),
		new TString("F_TEXT", text, sizeof text),
		new TLong("F_EXTRA_NO", &extraNo),
		new TString("F_EPORTAL_MAIL", eportalMail, sizeof eportalMail),
		new TLong("F_EPORTAL_MODE", &eportalMode),
		new TRadio("F_EPORTAL_REGISTER", &eportalMode, EPORTAL_REGISTER),
		new TRadio("F_EPORTAL_GRANT", &eportalMode, EPORTAL_GRANT),
		new TRadio("F_EPORTAL_REVOKE", &eportalMode, EPORTAL_REVOKE),
		NULL
	);

	descs = Descs + TABLE_INREG;
	key.type = TYPE_INREG;
	kind = *KIND_IN_REG_DOCS;
	antiNoRight = RIGHT_ANTIDOCNO;
}

TGroup *TInReg::Clone()
{
	return new TInReg;
}

TDecision *TInReg::AppealedDecision(bool reKind, bool reDate, const char *fields) const
{
	if (decisionKind && strchr(KIND_ENDOCS, decisionKind) && !extraNo)
	{
		TDecision *const decision = new TDecision;

		decision->key = lawsuit;
		decision->kind = decisionKind;
		decision->date = decisionDate;

		if (decision->Try(fields ? fields : "F_KIND, F_DATE, F_FROM_DATE, F_RESULT, F_EVENT_NO, F_SESSION_KIND")
			&& decision->sessionKind == sessionKind)	// 2016:072 TRQ: don't hit another session from same date
		{
			if (reKind)
				decision->kind = decision->IndexKind();

			if (reDate)
				decision->date = decision->fromDate;

			return decision;
		}

		delete decision;
	}

	return NULL;
}

sign TInReg::EPortalSendable() const
{
	// 2018-10-03: LPR: added session support
	if (XFER_EPORTAL)
	{
		TDecision *decision = AppealedDecision(false, false, "F_KIND, F_FLAGS");
		TVirtPtr VP(decision);

		if (decision)
			return decision->flags & DECISION_PUBLIC;

		if (sessionKind && strchr(KIND_SESSIONS, sessionKind))
		{
			TSession session;

			session.key = lawsuit;
			session.kind = sessionKind;
			session.date = decisionDate;

			if (session.Try("F_RESULT, F_FLAGS") && session.IsEPortSessEndoc())
				return -1;
		}
	}

	return false;
}

const char *TInReg::EPortalModeText() const
{
	switch (eportalMode)
	{
		case EPORTAL_GRANT : return "Разрешаване на достъп";
		case EPORTAL_REVOKE : return "Отнемане на достъп";
		default : return "Начална регистрация";
	}
}

TIndexContainer *TInReg::AppealedAct(bool reKind, bool reDate, const char *fields) const
{
	TDecision *tDecision = AppealedDecision(reKind, reDate, fields);

	if (tDecision)
		return tDecision;

	const char *tFields;
	TIndexContainer *container = NULL;

	// 2016:176 LPR: surrouendoc support
	if (extraNo)
	{
		TSurroundment *surrouendoc = new TSurroundment;

		surrouendoc->key.no = surroundNo;
		surrouendoc->key.year = surroundYear;
		surrouendoc->kind = surroundKind;
		surrouendoc->date = decisionDate;
		surrouendoc->lawsuit = lawsuit;
		surrouendoc->sessionKind = sessionKind;
		surrouendoc->decisionKind = decisionKind;
		surrouendoc->decisionDate = decisionDate;
		surrouendoc->extraNo = extraNo;
		tFields = "F_RESULT, F_EVENT_NO";
		container = surrouendoc;
	}
	else if (sessionKind && strchr(KIND_SESSIONS, sessionKind))
	{
		container = new TSession;
		container->key = lawsuit;
		container->kind = sessionKind;
		container->date = decisionDate;
		tFields = "F_KIND, F_DATE, F_RESULT, F_EVENT_NO";
	}
	else if (decision.type && strchr(TYPE_INREGS, decision.type))
	{
		container = new TInRegEvent;
		container->key = decision;
		container->kind = *KIND_INREG_EVENTS;
		container->date = decisionDate;
		tFields = "F_KIND, F_DATE, F_RESULT, F_EVENT_NO";
	}
	else if (decision.type && strchr(TYPE_REQUESTS, decision.type))
	{
		container = new TRequestResolution;
		container->key = lawsuit;
		container->kind = KIND_REQUEST_RESOLUTION;
		container->date = decisionDate;
		tFields = "F_KIND, F_DATE, F_RESOLUTION, F_EVENT_NO";
	}

	if (container)
	{
		if (container->Get(fields ? fields : tFields))
		{
			if (reKind)  // 2017:087 TRQ: decisionKind has priority over IndexKind
				container->kind = decisionKind ? kind2VisibleKind(decisionKind) : container->IndexKind();

			if (extraNo && reDate)
				container->date = ((TSurroundment *) container)->decisionDate;
		}
		else
		{
			delete container;
			container = NULL;
		}
	}

	return container;
}

bool TInReg::Insert()
{
	key.year = date.year;
	key.type = kind2type(kind);

	if (time.Empty())
		time = LNow();

	*RequestIndex << this;
	if (!RequestIndex->Insert())
		return false;
	*this << RequestIndex;

	if (lawsuit.no && !strchr(KIND_TEXTUAL_IN_DOCS, kind))
	{
		if (!TDatedContainer::Insert())
		{
			RequestIndex->Delete(true);
			return false;
		}

		return true;
	}

	if (!TRCDKeyGroup::Insert())
	{
		RequestIndex->Delete(true);
		return false;
	}

	Signal(ELECTRIC_ADD);
	return true;
}

void TInReg::Involvements(mstr &m) const
{
	if (strchr(KIND_TEXTUAL_IN_DOCS, kind))
		m.cat(kind == KIND_EPORTAL_REQUEST ? INVOLVEMENT_EPORTALS : C2S[INVOLVEMENT_SENDER]);
	else
	{
	#if 20170228
		// 2017:228 copied from/synced with TSurround
		TDatedContainer container("T_CONTAINER", NULL);

		container.kind = kind;
		container.key = lawsuit;
		container.Involvements(m);
	#else  // 20170228
		// 2007:282 LPR: skip Dated level to avoid inreg recursion
		TRCDKeyContainer::Involvements(s);

		TRCDKey tKey = key;
		key = lawsuit;
		TDatedContainer::Involvements(s + strlen(s));
		key = tKey;
	#endif  // 20170228
	}
}

TInRegEvent *TInReg::LastEvent(const char *fields, bool sendRecv) const
{
	msql m;
	TInRegEvent *inRegEvent = new TInRegEvent;

	LastKey(m, inRegEvent, fields);
	m.cat(", F_KIND");			// 2011:019 +F_KIND
	TQuery q(str(m));

	while (q.Read())
	{
		*inRegEvent << q;

		if (!sendRecv || inRegEvent->outNo)
			return inRegEvent;
	}

	delete inRegEvent;
	return NULL;
}

bool TInReg::LastReceiver(char *receiver, char *receiverType) const
{
	TInRegEvent *const inRegEvent = LastEvent("F_OUT_NO, F_OUT_YEAR", true);
	TVirtPtr VP(inRegEvent);

	if (inRegEvent)
	{
		TOutReg tOutReg;

		tOutReg.key.no = inRegEvent->outNo;
		tOutReg.key.year = inRegEvent->outYear;

		if (tOutReg.Try("F_RECEIVER, F_RECEIVER_TYPE"))
		{
			if (receiver)
				strcpy(receiver, tOutReg.receiver);

			if (receiverType)
				*receiverType = tOutReg.receiverType;

			return true;
		}
	}

	return false;
}

bool TInReg::Latest(TRCDKeyContainer *container)
{
	if (lawsuit.no)
		return TPrimeContainer::Latest(container);

	if (surroundNo)
	{
		// 2015:050 don't read surround, fill inline
		container->key.no = surroundNo;
		container->key.year = surroundYear;
		container->key.type = TYPE_SURROUND;
		container->date = date;
		return true;
	}

	return false;
}

const char *TInReg::SameComplect(const TInReg *leadInReg, char leadStatus, char status) const
{
	if (extraNo != leadInReg->extraNo)
		return "Не е за същото възражение";

	if (decisionDate != leadInReg->decisionDate || sessionKind != leadInReg->sessionKind)
		return "Не е за същото заседание или съдебен акт";

	if ((strchr(RESULT_COMPLECTS, leadStatus) != NULL) != (strchr(RESULT_COMPLECTS, status) != NULL))
		return "Не е от същия комплект";

	if (atob(receiver) && !TInReg::LastReceiver(NULL, NULL) && strcmp(receiver, leadInReg->receiver))
		return "Различни адресанти";

	return NULL;
}

char TInReg::Status() const
{
	TInRegEvent *lastEvent = LastEvent("F_RESULT");

	if (lastEvent)
	{
		char status = lastEvent->Status();
		delete lastEvent;
		return status;
	}

	return TIOReg::Status();
}

// ----- TOldCitizen ---------------------------------------------------------
TOldCitizen::TOldCitizen()
	: TCitizen("T_OLD_CITIZEN", TOldCitizen::Clone)
{
	descs = Descs + TABLE_OLD_CITIZEN;
}

TGroup *TOldCitizen::Clone()
{
	return new TOldCitizen;
}

// ----- TAccusation ---------------------------------------------------------
TAccusation::TAccusation()
	: TUCNGroup("T_ACCUSATION", TAccusation::Clone)
{
	Add(
		NEW_RCDKEY("F_", &key),
		new TText("F_ACCUSATION", &accusation, SIZE_OF_ACCUSATION_TEXT),
		new TCharAlias("F_CRIME_FORM", &crimeForm),
		new TCharAlias("F_CRIME_STAGE", &crimeStage),
		new TDate("F_CRIME_DATE", &crimeDate, false),
		new TCharAlias("F_RECIDIVE", &recidive),
		new TCharAlias("F_PUNISHMENT", &punishment),
		new TCharAlias("F_RESOLUTION", &resolution),
		new TLong("F_ACCUSATION_NO", &accusationNo),
		new TLong("F_FLAGS", &flags),
		new TCheck("F_FLAGS_OLD", &flags, ACCUSATION_OLD),
		new TDate("F_ARREST_DATE", &arrestDate, false),
		NULL
	);

	descs = Descs + TABLE_ACCUSATION;
	punishment = PUNISHMENT_NOT_KNOWN;
	resolution = RESOLUTION_FAKE;
	crimeForm = CRIME_FORM_NONE_YET;
	crimeStage = CRIME_STAGE_NONE_YET;
	recidive = RECIDIVE_NOT_KNOWN;
}

TGroup *TAccusation::Clone()
{
	return new TAccusation;
}

void TAccusation::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_UCN, F_UCN_TYPE, F_NO, F_YEAR, F_TYPE");
}

void TAccusation::FloatCriteria(msql &m, const char *fields)
{
	if (key.no)
	{
		m.Begin(Name, fields);
		m.Add(this, "F_NO, F_YEAR, F_TYPE");
	}
	else
		TUCNGroup::FloatCriteria(m, fields);
}

// ----- TArchive ------------------------------------------------------------
TArchive::TArchive()
	: TRCDKeyGroup("T_ARCHIVE", TArchive::Clone)
{
	Add(
		new TLong("F_LINK_NO", &linkNo),
		new TLong("F_ARCHIVE_NO", &archiveNo),
		new TDate("F_ARCHIVE_DATE", &archiveDate, false),
		new TLong("F_ARCHIVE_YEAR", &archiveYear),
		new TText("F_ARCHIVE_REMARK", &archiveRemark, SIZE_OF_ARCHIVE_TEXT),
		new TDate("F_KILL_DATE", &killDate, false),
		new TText("F_KILL_REMARK", &killRemark, SIZE_OF_ARCHIVE_TEXT),
		new TLong("F_KEEP", &keep),
		new TLong("F_KILLER_NO", &killerNo),
		new TDate("F_KILLER_DATE", &killerDate, false),
		new TText("F_REMAINDER", &remainder, SIZE_OF_REMAINDER_TEXT),
		new TLong("F_REMAINDER_NO", &remainderNo),
		new TLong("F_REMAINDER_YEAR", &remainderYear),
		new TStringAlias("F_ARCHIVE_INDEX", archiveIndex, sizeof archiveIndex), 
		new TString("F_ARCHIVE_INDEX_STRING", archiveIndex, sizeof archiveIndex),
		new TZLong("F_EXCESS", &excess),
		NULL
	);

	descs = Descs + TABLE_ARCHIVE;
}

TGroup *TArchive::Clone()
{
	return new TArchive;
}

void TArchive::ExactCriteria(msql &m, const char *fields)
{
	TRCDKeyGroup::ExactCriteria(m, fields);
	m.AddZLong("F_EXCESS", excess);
}

void TArchive::FloatCriteria(msql &m, const char *fields)
{
	TRCDKeyGroup::FloatCriteria(m, fields);
	m.AddZLong("F_EXCESS", excess);
}

bool TArchive::Generate()
{
	msql m(Name, "MAX(F_ARCHIVE_NO)");

	m.AddLong("F_ARCHIVE_YEAR", archiveYear);
	m.AddChar("F_TYPE", key.type);

	return generate_max(Name, str(m), &archiveNo, NO_MAX, strchr(Default->functions, RIGHT_ANTILAWNO) ? 0 : archiveYear);
}

bool TArchive::Insert()
{
	archiveYear = archiveDate.year;

	if (!Generate())
		return false;

	return TRCDKeyGroup::Insert();
}

void TArchive::Signal(int sigtype, const TExternContainer *lawsuit)
{
	if (!excess)
		electric_signal(sigtype, lawsuit);
}

char TArchive::Status() const
{
	return killDate.Empty() ? RESULT_ARCHIVED : RESULT_DESTROYED;
}

bool TArchive::TryBy(const TExternContainer *lawsuit, const char *fields)
{
	key = lawsuit->key;
	excess = lawsuit->Excess();
	return Try(fields);
}

// ----- TProveAction --------------------------------------------------------
TProveAction::TProveAction()
	: TDatedContainer("T_PROVE_ACTION", TProveAction::Clone)
{
	Add(
		new TCharAlias("F_RESOLUTION", &resolution),
		new TLong("F_FLAGS", &flags),
		new TCheck("F_FLAGS_INSTANCE", &flags, PROVE_INSTANCE),
		new TCharAlias("F_SESSION_KIND", &sessionKind),
		new TDate("F_SESSION_DATE", &sessionDate, false),
		new TCharAlias("F_DECISION_KIND", &decisionKind),
		new TLong("F_OUT_NO", &outNo),
		new TLong("F_OUT_YEAR", &outYear),
		new TText("F_TEXT", &text, SIZE_OF_PROVE_ACTION_TEXT),
		NULL
	);

	descs = Descs + TABLE_PROVE_ACTION;
	key.type = TYPE_PROVE;
	kind = KIND_PROVE_ACTION;
}

TGroup *TProveAction::Clone()
{
	return new TProveAction;
}

void TProveAction::Involvements(mstr &m) const
{
	m.clear();	// required?..
}

// ----- TProve --------------------------------------------------------------
TProve::TProve()
	: TPrimeContainer("T_PROVE", TProve::Clone)
{
	Add(
		new TText("F_DESCRIPTION", &description, SIZE_OF_PROVE_DESCRIPTION),
		new TString("F_REMARK", remark, sizeof remark),
		NULL
	);

	descs = Descs + TABLE_PROVE;
	key.type = TYPE_PROVE;
	kind = KIND_PROVE;
}

TGroup *TProve::Clone()
{
	return new TProve;
}

void TProve::ExactCriteria(msql &m, const char *fields)
{
	TRCDKeyContainer::ExactCriteria(m, fields);
}

bool TProve::Insert()
{
	key.year = date.year;
	return Generate() && TDatedContainer::Insert();
}

TProveAction *TProve::LastAction(const char *fields, const CDate *date) const
{
	TProveAction *proveAction = new TProveAction;
	msql m(proveAction->Name, fields);

	m.Add(this, "F_NO, F_YEAR, F_TYPE");
	if (date)
	{
		m.Add("F_DATE");
		m.printf("<= %ld", date->Value());
	}
	m.AddOrder("F_DATE DESC");
	TQuery q(str(m));

	if (q.Read())
		*proveAction << q;
	else
	{
		delete proveAction;
		proveAction = NULL;
	}

	return proveAction;
}

char TProve::Status() const
{
	TProveAction *const lastAction = LastAction("F_RESOLUTION");
	TVirtPtr VP(lastAction);
	return lastAction ? lastAction->resolution : TPrimeContainer::Status();
}

bool TProve::TryBy(const TRCDKeyGroup *group, const char *fields)
{
	msql m;

	lawsuit = group->key;
	FloatCriteria(m, fields);

	TQuery q(str(m));
	if (q.Read())
	{
		*this << q;
		return true;
	}

	return false;
}

// ----- TSched --------------------------------------------------------------
TSched::TSched()
	: TDatedContainer("T_SCHED", TSched::Clone)
{
	Add(
		new TCharAlias("F_SCHED", &sched),
		new TDate("F_START", &start, false),
		new TDate("F_FINAL", &final, false),
		NEW_RCDKEY("F_LAWSUIT_", &lawsuit),
		new TPersonAlias("F_JUDGE", judge, sizeof judge),
		NULL
	);

	descs = Descs + TABLE_SCHED;
}

TGroup *TSched::Clone()
{
	return new TSched;
}

void TSched::ExactCriteria(msql &m, const char *fields)
{
	TDatedContainer::ExactCriteria(m, fields);
	m.Add(this, "F_SCHED");
}

void TSched::FloatCriteria(msql &m, const char *fields)
{
	TDatedContainer::ExactCriteria(m, fields);
	m.AddOrder("F_FINAL");
}

bool TSched::Insert()
{
	TSched tSched;

	if (!atob(judge))
		*judge = '\0';

	tSched << this;

	if (tSched.Try())
	{
		if (tSched.final < final || strcmp(tSched.judge, judge))
		{
			tSched.start = start;
			tSched.final = final;
			strcpy(tSched.judge, judge);
			return tSched.Update("F_START, F_FINAL, F_JUDGE");
		}
		else
			return true;
	}

	return TGroup::Insert();
}

bool TSched::Update(const char *fields)
{
	return TGroup::Update(fields);
}

bool TSched::Delete(bool exact)
{
	return TGroup::Delete(exact);
}

void TSched::Print(mstr &m)
{
	m.printf("%-40s %-D %-D", CalInterval->Seek(sched), &start, &final);
}

// ----- TExpense ------------------------------------------------------------
TExpense::TExpense()
	: TWindData("T_EXPENSE", TExpense::Clone)
{
	Add(
		new TLong("F_EXPENSE", &expense),
		new TCharAlias("F_SUM_TYPE", &sumType),
		new TCharAlias("F_SUM_STATE", &sumState),
		new TCharAlias("F_ACCOUNT_TYPE", &accountType),
		new TString("F_TEXT", text, sizeof text),
		new TLongAlias("F_PENALITY_TYPE", &penalityType),
		new TDate("F_REJECT_DATE", &rejectDate, false),
		new TCharAlias("F_REJECT_KIND", &rejectKind),
		new TLong("F_FLAGS", &flags),
		new TStringAlias("F_REJECT_COURT", rejectCourt, sizeof rejectCourt),
		new TLongAlias("F_REASON_TYPE", &reasonType),
		new TMoney("F_SUM_1", &sum1),
		new TMoney("F_SUM_2", &sum2),
		new TDate("F_PAID_DATE", &paidDate, false),
		NEW_RCDKEY("F_PAID_", &paid),
		new TString("F_PAID_TRANS_ID", paidTransId, sizeof paidTransId),
		new TString("F_PAID_NOTE", paidNote, sizeof paidNote),
		NULL
	);

	descs = Descs + TABLE_EXPENSE;
	paid.type = TYPE_SURROUND;
}

TGroup *TExpense::Clone()
{
	return new TExpense;
}

void TExpense::ExactCriteria(msql &m, const char *fields)
{
	TExpense::FloatCriteria(m, fields);
	m.Add(this, "F_EXPENSE");
}

void TExpense::FloatCriteria(msql &m, const char *fields)
{
	TWindData::ExactCriteria(m, fields);
	m.Add(this, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE");
}

void TExpense::Generate()
{
	msql m;

	TExpense::FloatCriteria(m, "MAX(F_EXPENSE) AS F_EXPENSE");
	expense = dataBase->GetLong(str(m), "F_EXPENSE");

	if (++expense > EXPENSE_MAX)
		generator_limit(Name);
}

bool TExpense::Insert()
{
	if (!expense)
		Generate();

	return TWindData::Insert();
}

void TExpense::Print(mstr &m)
{
	m.printf("%-15s %12.2lf %-15s %s", SumType->Seek(sumType), Sum(), SumState->Seek(sumState),
		sumType == SUM_TYPE_PENALITY ? PenalityType->Seek(penalityType) : text);
}

// ----- TIndex --------------------------------------------------------------
TIndex::TIndex(char type, TGroup *(*const tClone)())
	: TGroup("T_INDEX", tClone)
{
	Add(
		new TLong("F_NO", &no),
		new TLong("F_YEAR", &year),
		new TCharAlias("F_INDEX", &index),
		NULL
	);

	descs = Descs + TABLE_INDEX;
	index = type;
}

void TIndex::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_NO, F_YEAR, F_INDEX");
}

void TIndex::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_YEAR, F_INDEX");
}

bool TIndex::Generate()
{
	msql m(Name, "MAX(F_NO)");

	m.AddLong("F_YEAR", year);
	m.AddChar("F_INDEX", index);

	return generate_max(Name, str(m), &no, XFER_NO_MAX, strchr(Default->functions, RIGHT_ANTIDOCNO) ? 0 : year);
}

bool TIndex::Insert()
{
	if (!Generate())
		return false;

	return TGroup::Insert();
}

// ----- TRequestIndex -------------------------------------------------------
TRequestIndex::TRequestIndex()
	: TIndex(TYPE_REQUEST, TRequestIndex::Clone)
{
}

TGroup *TRequestIndex::Clone()
{
	return new TRequestIndex();
}

TRequestIndex *RequestIndex = NULL;

// ----- TAnnounceIndex ------------------------------------------------------
TAnnounceIndex::TAnnounceIndex()
	: TIndex(TYPE_ANNOUNCE, TAnnounceIndex::Clone)
{
}

TGroup *TAnnounceIndex::Clone()
{
	return new TAnnounceIndex();
}

TAnnounceIndex *AnnounceIndex = NULL;	// 2012:039

// ----- TEventIndex ---------------------------------------------------------
TEventIndex::TEventIndex()
	: TGroup("T_EVENT_INDEX", TEventIndex::Clone)
{
	Add(
		new TLong("F_EVENT_NO", &eventNo),
		new TLong("F_YEAR", &year),
		new TCharAlias("F_TYPE", &type),
		new TCharAlias("F_KIND", &kind),
		NULL
	);

	descs = Descs + TABLE_EVENT_INDEX;
}

TGroup *TEventIndex::Clone()
{
	return new TEventIndex;
}

void TEventIndex::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_EVENT_NO, F_YEAR, F_TYPE, F_KIND");
}

void TEventIndex::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_YEAR, F_TYPE, F_KIND");
}

bool TEventIndex::Generate()
{
	msql m;
	TEventIndex::FloatCriteria(m, "MAX(F_EVENT_NO)");
	return generate_max(Name, str(m), &eventNo, EVENT_NO_MAX, strchr(Default->functions, RIGHT_ANTIEVTNO) ? 0 : year);
}

bool TEventIndex::Insert()
{
	if (!Generate())
		return false;

	return TGroup::Insert();
}

TEventIndex *DecisionIndex = NULL;

// ----- TConnect ------------------------------------------------------------
TConnect::TConnect()
	: TGroup("T_CONNECT", TConnect::Clone)
{
	Add(
		NEW_RCDKEY("F_", &key),
		new TCharAlias("F_UCN_TYPE", &ucnType),
		new TStringAlias("F_UCN", ucn, sizeof ucn),
		new TString("F_CONNECT_NO", connectNo, sizeof connectNo),
		new TLong("F_CONNECT_YEAR", &connectYear),
		new TCharAlias("F_CONNECT_TYPE", &connectType),
		new TCharAlias("F_CONNECT_KIND", &connectKind),
		new TLongAlias("F_COMPOSITION", &composition),
		new TCharAlias("F_DECISION_KIND", &decisionKind),
		new TDate("F_DECISION_DATE", &decisionDate, false),
		new TCharAlias("F_OUT_KIND", &outKind),
		new TLong("F_OUT_NO", &outNo),
		new TLong("F_OUT_YEAR", &outYear),
		new TString("F_TEXT", text, sizeof text),
		new TLongAlias("F_XFER_KIND", &xferKind),
		new TLong("F_DIRTY", &dirty),
		NULL
	);

	descs = Descs + TABLE_CONNECT;
}

TGroup *TConnect::Clone()
{
	return new TConnect;
}

const char *TConnect::CompositionName()
{
	TSender sender;

	strcpy(sender.ucn, ucn);
	sender.ucnType = ucnType;

	return sender.Get() ? sender.court == COURT_CODE ? Composition->Seek(composition) : romanian_name(composition) : "";
}

void TConnect::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_NO, F_YEAR, F_TYPE, F_CONNECT_TYPE");
}

void TConnect::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_NO, F_YEAR, F_TYPE");
}

void TConnect::Print(mstr &m, bool alias, bool cyrno, constant compos)
{
	TSender sender;
	size_t pos = m.len();

	sender.ucnType = ucnType;
	strcpy(sender.ucn, ucn);
	if (!sender.Get())
		strcpy(sender.name, sender.ucn);

	m.cat(ConnectType->Seek(connectType));

	if (connectKind)
	{
		ConnectKind->Seek(connectKind);
		const char *name = alias ? ConnectKind->alias : ConnectKind->name;

		if (!strcmp(ConnectType->name, ConnectKind->name))
		{
			m.cut(pos);
			m.cat(name);
		}
		else
		{
			if (!strcmp(m.end() - 5, " дело") && strstr(ConnectKind->name, " дело"))
				m.cut(m.end() - 5);

			m.printf(" %s", name);
		}
	}

	if (cyrno)
		m.cat(" №");

	m.printf(" %s/%ldг. ", connectNo, connectYear);
	if (compos && composition)
		m.printf("%s %s ", CompositionName(), compos);
	m.cat(sender.name);
}

void TConnect::Print(mstr &m)
{
	Print(m, false, false, NULL);

	if (composition)
		m.printf(" %s състав", CompositionName());
}

bool TConnect::TryBy(const TRCDKey &tKey, char tConnectType, const char *fields)
{
	key = tKey;
	connectType = tConnectType;
	return Try(fields);
}

bool TConnect::TryBy(const TRCDKeyGroup *group, char connectType, const char *fields)
{
	return TryBy(group->key, connectType, fields);
}

bool TConnect::TryBy(const TRCDKeyGroup *group, const char *connectTypes, const char *fields)
{
	msql m(Name, fields);

	m.Add(group, "F_NO, F_YEAR, F_TYPE");
	m.AddChars("F_CONNECT_TYPE", connectTypes);

	TQuery q(str(m));

	if (q.Read())
	{
		*this << q;
		return true;
	}

	return false;
}

bool TConnect::TryLower(const TRCDKey &tKey, const char *fields)
{
#if COURT_TYPE == COURT_APPEAL
	if (TryBy(tKey, CONNECT_TYPE_RECALL_LAWSUIT, fields))
		return true;
#endif  // APPEAL
	return TryBy(tKey, CONNECT_TYPE_LOWER_INSTANCE, fields);
}

const char *TConnect::Returnable(TConnectReturnable returnable) const
{
	bool lower = (returnable & RETURNABLE_LOWER) && (XFER_ACTIVE & XFER_REQLAWS);
	bool eispp = (returnable & RETURNABLE_EISPP) && EISPP_ACTIVE;

	if (lower || eispp)
	{
		if (lower && (!outNo || !outYear || !connectKind))
		{
			return connectType == CONNECT_TYPE_LOWER_INSTANCE ?
				"Данните, с които документа/делото е получено от първоинстанционния съд, са некомплектни" :
				"Данните, с които документа/делото е получено от въззивния съд, са некомплектни";
		}

		TSender sender;
		sender.ucnType = ucnType;
		strcpy(sender.ucn, ucn);

		if (!sender.Get())
			return "Липсващ адресант";

		if (sender.court)
		{
			if (key.type == TYPE_PUNISHMENT_LAW && eispp && !sender.eispp)
			{
				return connectType == CONNECT_TYPE_LOWER_INSTANCE ?
					"Липсва ЕИСПП код на първоинстанционния съд" :
					"Липсва ЕИСПП код на въззивния съд";
			}
		}
		else if (!strchr(UCN_EXECUTORS, sender.ucnType))
		{
			return connectType == CONNECT_TYPE_LOWER_INSTANCE ?
				"Адресантът по първоинстанционното дело не е съд или частен съдебен изпълнител" :
				"Адресантът по въззивното дело не е съд или частен съдебен изпълнител";
		}
	}

	return NULL;
}

bool TConnect::ReturnedTo() const
{
	TOutReg outReg;
	msql m(outReg.Name, "F_NO");

	outReg.lawsuit = key;
	outReg.receiverType = ucnType;
	strcpy(outReg.receiver, ucn);
	m.Add(&outReg, "F_LAWSUIT_NO, F_LAWSUIT_YEAR, F_LAWSUIT_TYPE, F_RECEIVER, F_RECEIVER_TYPE");

	TQuery q(str(m));
	return q.Read();
}

// ----- TReplacePair --------------------------------------------------------
TReplacePair::TReplacePair(const char *tFind, const char *tReplace)
{
	strcpy(find, tFind);
	strcpy(replace, tReplace);
}

void TReplacePair::FindReplace(char *text, IDispatch *iActive) const
{
	char *s = text;
	int replen = strlen(replace);

	// glob case-insensitive search
	while (*s)
	{
		char *end = globcmpi(s, find);

		if (!end)
		{
			s++;
			continue;
		}

		if (iActive && !filter_replace(iActive, text, s - text, end - s, replace, replen))
			break;

		strmove(s, end);
		if (replen)
		{
			strmove(s + replen, s);
			memcpy(s, replace, replen);
			s += replen;
		}
	}
}

void TReplacePair::Print(mstr &m) const
{
	m.cpy(find);
	m.setlen(LENGTH_OF_FIND_REPLACE_LIST);
	m.cach('|');
	m.cat(replace);
	m.setlen(LENGTH_OF_FIND_REPLACE_LIST + 1 + LENGTH_OF_FIND_REPLACE_LIST);
}

// ----- TReplacePairs -------------------------------------------------------
TReplacePairs::TReplacePairs()
	: TRCDKeyGroup("T_REPLACE_PAIRS", NULL)
{
	Add(new TText("F_TEXT", &text, SIZE_OF_REPLACE_PAIRS_TEXT));
	descs = Descs + TABLE_REPLACE_PAIRS;
}

void TReplacePairs::FloatCriteria(msql &m, const char *fields)
{
	TReplacePairs::ExactCriteria(m, fields);
}

bool TReplacePairs::Try(const char *fields)
{
	if (TRCDKeyGroup::Try(fields))
	{
		// char remapping only
		char *beg = ncstr(text);
		char *s, *v;
		int n = 0;

		while ((s = strchr(beg, '\n')) != NULL)
		{
			*s = '\0';
			if ((v = strchr(beg, '\v')) != NULL)
				*v++ = '\0';
			else
				v = s;
				
			if (find_replace_valid(beg, v, false))
			{
				TReplacePair *pair = new TReplacePair(beg, v);
				if (++n == FIND_REPLACES_MAX)
					break;
				pairs.Add(pair);
			}
			beg = s + 1;
		}

		return true;
	}

	return false;
}

// ----- TLawInterval --------------------------------------------------------
TLawInterval::TLawInterval()
	: TRCDKeyGroup("T_LAW_INTERVAL", TLawInterval::Clone)
{
	Init();
}

TLawInterval::TLawInterval(const TRCDKey &tKey, const CDate &tDate)
	: TRCDKeyGroup("T_LAW_INTERVAL", TLawInterval::Clone)
{
	Init();
	key = tKey;
	date = tDate;
}

void TLawInterval::Init()
{
	Add(
		new TDate("F_DATE", &date, false),
		new TLong("F_FLAGS", &flags),
		new TCheck("F_FLAGS_SECONDARY", &flags, LAW_INTERVAL_SECONDARY),
		new TDate("F_FIRST_SET", &firstSet, false),
		new TCharAlias("F_AGE", &age),
		new TDate("F_FINISHED", &finished, false),
		new TCharAlias("F_GRANTED", &grant),
		NULL
	);

	descs = Descs + TABLE_LAW_INTERVAL;
}

TGroup *TLawInterval::Clone()
{
	return new TLawInterval;
}

void TLawInterval::ExactCriteria(msql &m, const char *fields)
{
	TLawInterval::FloatCriteria(m, fields);
	m.Add(this, "F_DATE");
}

void TLawInterval::FloatCriteria(msql &m, const char *fields)
{
	TRCDKeyGroup::ExactCriteria(m, fields);
}

bool TLawInterval::Update(const char *fields)
{
	if (!TRCDKeyGroup::Update(fields))
		return false;

	if ((flags & LAW_INTERVAL_SECONDARY) == 0)
	{
		TLawsuit lawsuit;
		mstr tFields(fields);

		lawsuit << this;

		if (strstr(fields, "F_FINISHED") && finished.Empty())
			tFields.cat(", F_DEC_MADE, F_FORCE_DATE");

		lawsuit.Update(str(tFields));
	}

	return true;
}

bool TLawInterval::Read(const char *fields, bool silent)
{
	msql m(Name, fields);

	m.Add(this, "F_NO, F_YEAR, F_TYPE");
	m.Add("F_DATE");
	m.printf("<= %ld", date.Value());
	m.AddOrder("F_DATE DESC");

	TQuery q(str(m));
	if (q.Read())
	{
		*this << q;
		return true;
	}

	return silent ? false : say_no_data(Name);
}

bool TLawInterval::Finished(const TLawsuit *lawsuit, const CDate &date)
{
	TLawInterval interval(lawsuit->key, date);
	return interval.Read("F_FINISHED", true) && !interval.finished.Empty();
}

bool TLawInterval::operator<=(const TLawInterval &that) const
{
	return date <= that.date;
}

// ----- TJurorDistrib -------------------------------------------------------
TJurorDistrib::TJurorDistrib()
	: TRCDKeyGroup("T_JUROR_DISTRIB", NULL)
{
	Add(
		new TPersonAlias("F_JUROR", juror, sizeof juror),
		new TPersonAlias("F_REJUROR", reJuror, sizeof reJuror),
		new TPersonAlias("F_JUROR_X", jurorX, sizeof jurorX),
		new TPersonAlias("F_RESERVE", reserve, sizeof reserve),
		new TText("F_MOTIVES", &motives, SIZE_OF_MOTIVES),
		new TDate("F_DATE", &date, false),
		new TString("F_MD5SUM", md5sum, sizeof md5sum),
		new TZLong("F_EXCESS", &excess),
		new TZLong("F_CLOSED", &closed),
		new TTime("F_TIME", &time),
		NULL
	);

	descs = Descs + TABLE_JUROR_DISTRIB;
	jurors[0] = juror;
	jurors[1] = reJuror;
	jurors[2] = jurorX;
	jurors[3] = reserve;
	lawsuitKind = '\0';
}

void TJurorDistrib::CalcMD5(char *tMD5Sum)
{
	char buffer[LENGTH_OF_MOTIVES + 100];
	char *ptr = buffer;

	memcpy(ptr, juror, LENGTH_OF_UCN);
	ptr += LENGTH_OF_UCN;
	memcpy(ptr, &key, sizeof key);
	ptr += sizeof key;
	memcpy(ptr, reJuror, LENGTH_OF_UCN);
	ptr += LENGTH_OF_UCN;
	if (atob(jurorX))
	{
		memcpy(ptr, jurorX, LENGTH_OF_UCN);
		ptr += LENGTH_OF_UCN;
	}
	if (atob(reserve))
	{
		memcpy(ptr, reserve, LENGTH_OF_UCN);
		ptr += LENGTH_OF_UCN;
	}

	struct TPackedDate
	{
		long year;
		char month;
		char day;
	} pDate = { date.year << excess, (char) date.month, (char) date.day };

	// 2017:142
	cac_assert(closed == !time.Empty());
	if (closed)
		pDate.year += time.minute * 10000 + time.hour * 1000000;

	memcpy(ptr, str(motives), motives.len());
	ptr += motives.len();
	memcpy(ptr, &pDate, sizeof pDate);
	ptr += sizeof pDate;

	if ((ptr - buffer) & 0x01)
		*ptr++ = 'x';

	calc_final(buffer, ptr, juror, sizeof buffer, tMD5Sum);
}

void TJurorDistrib::ExactCriteria(msql &m, const char *fields)
{
	TRCDKeyGroup::ExactCriteria(m, fields);
	m.AddZLong("F_EXCESS", excess);
	m.AddDate("F_DATE", date);
	m.AddZLong("F_CLOSED", closed);
}

void TJurorDistrib::FloatCriteria(msql &m, const char *fields)
{
	TRCDKeyGroup::ExactCriteria(m, fields);
	m.AddZLong("F_EXCESS", excess);
}

bool TJurorDistrib::Find(const TBaseSession *session, bool complain)
{
	msql m;

	key = session->key;
	excess = session->Excess();
	closed = strchr(KIND_CLOSED_SESSIONS, session->kind) != NULL;

	if (closed)
	{
		date = session->date;
		time = session->time;
		ExactCriteria(m);
	}
	else
	{
		FloatCriteria(m);
		m.AddZLong("F_CLOSED", FALSE);
		m.AddOrder("F_DATE DESC");
	}

	TQuery q(str(m));
	if (q.Read())
	{
		*this << q;
		return true;
	}

	date.Clear();
	return complain ? say_no_data(Name) : false;
}

bool TJurorDistrib::Insert()
{
	Regenerate();
	CalcMD5(md5sum);
	return TRCDKeyGroup::Insert();
}

void TJurorDistrib::Regenerate()
{
	motives.clear();
	srand(GetTickCount());
	motives.cat("*");

	for (int i = 0; i < 40; i++)
	{
		char value = (char) rand();
		motives.cach(strchr(" \t\n\177\230", value) ? '*' : value);
	}
}

bool TJurorDistrib::Try(const char *fields)
{
	if (TRCDKeyGroup::Try(fields))
	{
		if (!fields)
		{
			char tMD5Sum[SIZE_OF_MD5SUM];

			CalcMD5(tMD5Sum);
			if (strcmp(md5sum, tMD5Sum))
				fatal("%s: невалидна контролна сума.", Name);
		}

		return true;
	}

	return false;
}

bool TJurorDistrib::Update(const char *fields)
{
	Regenerate();
	CalcMD5(md5sum);
	return TRCDKeyGroup::Update(fields);
}

// ----- TSubServeData -------------------------------------------------------
TSubServeData::TSubServeData(TGroup *self)
{
	self->Add(
		new TDate("F_SUBPOENA_RECEIVED", &subpoenaReceived, false),
		new TTime("F_RECEIVED_TIME", &receivedTime),
		new TCharAlias("F_RECEIVED_STATUS", &receivedStatus),
		new TString("F_RECEIVED_TEXT", receivedText, sizeof receivedText),
		new TDate("F_SUBPOENA_RETURNED", &subpoenaReturned, false),
		NULL
	);
}

bool TSubServeData::operator==(const TSubServeData &that) const
{
	return subpoenaReceived == that.subpoenaReceived && receivedTime == that.receivedTime &&
		receivedStatus == that.receivedStatus && strcmp(receivedText, that.receivedText) == 0 &&
		subpoenaReturned == that.subpoenaReturned; 
}

// ----- TAnnounce -----------------------------------------------------------
int TAnnounceKey::Kompare(const TAnnounceKey &that) const
{
	return kind != that.kind ? kind - that.kind : year != that.year ? year - that.year : no - that.no;
}

TAnnounce::TAnnounce()
	: TGroup("T_ANNOUNCE", TAnnounce::Clone), TSubServeData(this), subpoena(NULL)
{
	Add(
		new TLong("F_NO", &key.no),
		new TLong("F_YEAR", &key.year),
		new TCharAlias("F_KIND", &key.kind),
		new TLongAlias("F_COURT", &court),
		new TLong("F_ANNOUNCE", &announce),
		new TLong("F_REQUEST_ID", &requestId),
		new TDate("F_SUBPOENA_DATE", &subpoenaDate, false),
		new TCharAlias("F_SUBPOENA_KIND", &subpoenaKind),
		new TDate("F_SESSION_DATE", &sessionDate, false),
		new TLong("F_SIDE_NO", &sideNo),
		new TString("F_SUBPOENA_TEXT", subpoenaText, sizeof subpoenaText),
		new TPersonAlias("F_DELIVERER_UCN", delivererUcn, sizeof delivererUcn),
		new TLongAlias("F_COMPOSITION", &composition),
		new TString("F_BARCODE", barcode, sizeof barcode),
		new TCoord("F_COORD_N", &coordN),	// 2015:348 was missing
		new TCoord("F_COORD_E", &coordE),	// 2015:348 was missing
		// 2010:159 FIX: transfer for list window edit
		// 2018-03-21: transfer causes double delete
		//new TLong("F_SUBPOENA", (long *) &subpoena),
		NULL
	);

	descs = Descs + TABLE_ANNOUNCE;
	AddFields(this);
	uclp = COURT_UCLP;
}

TGroup *TAnnounce::Clone()
{
	return new TAnnounce;
}

TAnnounce::~TAnnounce()
{
	delete subpoena;
}

void TAnnounce::GetSenderName(mstr &m) const
{
	get_sencor_name(SenderType(), court, m);
}

void TAnnounce::SetSubpoena(TSubpoena *tSubpoena)
{
#if RANGECHECK
	if (subpoena)
		fatal("secondary SetSubpoena()");
#endif  // RANGECHECK

	subpoena = tSubpoena;

	*this << subpoena;
	sessionDate = subpoena->date;
	court = COURT_CODE;

	TRCDKey origin;

	if (subpoena->FindOrigin(origin))
	{
		key.kind = type2ConnectKind(origin.type);
		key.no = origin.no;
		key.year = origin.year;
	}
	else
		fatal("unable to find origin for %d/%d/%d", subpoena->key.type, subpoena->key.no, subpoena->key.year);
}

void TAnnounce::ExactCriteria(msql &m, const char *fields)
{
	TAnnounce::FloatCriteria(m, fields);
	m.Add(this, "F_ANNOUNCE");
}

void TAnnounce::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_NO, F_YEAR, F_KIND, F_COURT");
}

bool TAnnounce::Insert()
{
	msql m;

	TAnnounce::FloatCriteria(m, "MAX(F_ANNOUNCE)");
#if TESTVER  // 2018-07-09
	announce = dataBase->GetLong(str(m), "MAX");
#else  // TESTVER
	TGetGroup g(str(m), new TLong("MAX", &announce));
#endif  // TESTVER

	if (++announce > ANNOUNCE_MAX)
		generator_limit(Name);

	if (court == COURT_CODE)
		requestId = TSubpoena::GenerateRequestId(DataBase);

	if (any(barcode))
	{
		msql m(Name);

		m.Add(this, "F_BARCODE");
		TQuery q(str(m));

		if (q.Read())
			return error("Вече има призовка/съобщение с този баркод.");
	}

	return TGroup::Insert();
}

void TAnnounce::Print(mstr &m)
{
	const char *sep = "";

	ConnectKind->Seek(key.kind);
	SubpoenaKind->Seek(subpoenaKind);
	m.printf("%c %-8s %5ld/%4ld %-15s ", strchr(SRS_DELIVEREDS, receivedStatus) ?
		tolower(*ReceivedStatus->Seek(receivedStatus)) : ' ',
		ConnectKind->alias, key.no, key.year, SubpoenaKind->alias);

	if (atob(delivererUcn))
	{
		print_user(m, delivererUcn);
		sep = ", ";
	}

	if (uclp)
	{
		TAddress address;
		size_t pos;

		m.cat(sep);
		pos = m.len();
		address << this;
		address.Print(m, ADDRESS_SKIP_LN1);
		m.compress(pos);
		sep = ", ";
	}

	if (subpoena)
	{
		m.cat(sep);
		subpoena->PrintNotifieds(m, ", ", " ");
	}
	else if (sideNo)
	{
		m.cat(sep);
		m.printf("№ на лице %ld", sideNo);
	}
}

bool TAnnounce::Less0(const TAnnounce *that) const
{
	return key.year != that->key.year ? key.year < that->key.year : key.no < that->key.no;
}

bool TAnnounce::Less1(const TAnnounce *that) const
{
	return key.kind != that->key.kind ? key.kind < that->key.kind : Less0(that);
}

bool TAnnounce::Less2(const TAnnounce *that) const
{
	return subpoenaKind != that->subpoenaKind ? subpoenaKind < subpoenaKind : Less0(that);
}

// ----- TBankrupt -----------------------------------------------------------
TBankrupt::TBankrupt()
	: TUCNGroup("T_BANKRUPT", TBankrupt::Clone)
{
	Add(
		new TString("F_CREDITOR", creditor, sizeof creditor),
		new TString("F_BULSTAT", bulstat, sizeof bulstat),
		NULL
	);

	descs = Descs + TABLE_BANKRUPT;
}

TGroup *TBankrupt::Clone()
{
	return new TBankrupt;
}

void TBankrupt::ExactCriteria(msql &m, const char *fields)
{
	TBankrupt::FloatCriteria(m, fields);
	m.Add(this, "F_UCN, F_UCN_TYPE");
}

void TBankrupt::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_CREDITOR");
}

void TBankrupt::Print(mstr &m)
{
	TFirm firm;
	firm << this;

	if (firm.Get())
	{
		m.cat(firm.name);
		m.pad(50);
		m.printf(" %s", firm.bulstat);
	}
	else
		m.printf("%3s %10s", UCNType->Seek(firm.ucnType), firm.ucn);
}

// ----- TInformed -----------------------------------------------------------
TInformed::TInformed()
	: TRCDKeyGroup("T_INFORMED", TInformed::Clone)
{
	Add(
		new TString("F_UCN", ucn, sizeof ucn),
		new TString("F_EMAIL", email, sizeof email),
		NULL
	);

	descs = Descs + TABLE_INFORMED;
}

TGroup *TInformed::Clone()
{
	return new TInformed;
}

void TInformed::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_NO, F_YEAR, F_TYPE, F_UCN");
}

void TInformed::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_UCN");
}

void TInformed::Print(mstr &m)
{
	Type->Seek(key.type);
	m.printf("%-5s  %5ld/%ld", Type->alias, key.no, key.year);
}

// ----- TSessionTimes -------------------------------------------------------
TSessionTimes::TSessionTimes()
	: TRCDKeyGroup("T_SESSION_TIMES", TSessionTimes::Clone)
{
	Add(
		new TCharAlias("F_KIND", &kind),
		new TDate("F_DATE", &date, false),
		new TDateTime("F_START", &start),
		new TTime("F_ENTIME", &entime),
		new TLong("F_HOURS", &hours),
		new TZLong("F_EXCESS", &excess),
		NULL
	);

	descs = Descs + TABLE_SESSION_TIMES;
}

TGroup *TSessionTimes::Clone()
{
	return new TSessionTimes;
}

void TSessionTimes::ImportFromSession(const TBaseSession *session)
{
	ImportFromGroup(session);
	excess = session->Excess();
}

void TSessionTimes::ExactCriteria(msql &m, const char *fields)
{
	TSessionTimes::FloatCriteria(m, fields);
	m.Add(this, "F_START");
}

void TSessionTimes::FloatCriteria(msql &m, const char *fields)
{
	TRCDKeyGroup::ExactCriteria(m, fields);
	m.Add(this, "F_KIND, F_DATE");
	m.AddZLong("F_EXCESS", excess);
}

const char *TSessionTimes::Invalid(bool internal) const
{
	return session_times_invalid(internal, start, start, entime, hours);
}

// ----- TEisPending ---------------------------------------------------------
TEisPending::TEisPending()
	: TGroup("T_EISPP_PENDING", TEisPending::Clone)
{
	Add(
		new TString("F_FILENAME", fileName, sizeof fileName),
		new TLong("F_REQUEST_ID", &requestId),
		NULL
	);

	descs = Descs + TABLE_EISPP_PENDING;
}

TGroup *TEisPending::Clone()
{
	return new TEisPending;
}

void TEisPending::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.AddString("F_FILENAME", fileName);
}

// ----- TPayment ------------------------------------------------------------
TPayment::TPayment()
	: TGroup("T_PAYMENT", TPayment::Clone)
{
	Add(
		new TString("F_TRANS_ID", transId, sizeof transId),
		new TLong("F_TERM_ID", &termId),
		new TLong("F_ECR", &ecr),
		new TLong("F_PAYMENT", &payment),
		new TMoney("F_SUM", &sum),
		new TDate("F_DATE", &date, true),
		new TTime("F_TIME", &time),
		new TCharAlias("F_SUM_TYPE", &sumType),
		new TString("F_UCN", ucn, sizeof ucn),
		new TCharAlias("F_UCN_TYPE", &ucnType),
		new TString("F_OBL", obl, sizeof obl),
		new TCharAlias("F_OBL_TYPE", &oblType),
		new TLong("F_COURT", &court),
		new TLong("F_NO", &key.no),
		new TLong("F_YEAR", &key.year),
		new TCharAlias("F_CONNECT_KIND", &key.kind),
		new TCharAlias("F_ACCOUNT_TYPE", &accountType),
		new TString("F_TEXT", text, sizeof text),
		new TString("F_AUTH_ID", authId, sizeof authId),
		new TString("F_CARD_ID", cardId, sizeof cardId),
		NULL
	);

	descs = Descs + TABLE_PAYMENT;
	termId = -1;
	ecr = POSTERM_ECR;
	strcpy(transId, TRANS_ID_PENDING);
	court = COURT_CODE;
}

TGroup *TPayment::Clone()
{
	return new TPayment;
}

void TPayment::ExactCriteria(msql &m, const char *fields)
{
	TPayment::FloatCriteria(m, fields);
	m.Add(this, "F_PAYMENT");
}

void TPayment::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	// 2018-10-23: for trans id zero
	//m.Add(this, "F_TRANS_ID, F_TERM_ID");
	// 2018-11-26: LPR: +F_AUTH_ID
	m.Add(this, "F_TRANS_ID, F_UCN, F_UCN_TYPE, F_DATE, F_AUTH_ID");
	m.AddRange("F_TIME", time.Value() - 1, time.Value() + 1);
}

void TPayment::Generate()
{
	ecr = POSTERM_ECR;	// these may be <<-ed
	if (!strcmp(transId, TRANS_ID_PENDING))
		termId = -1;	// from another payment

	date = Today;
	time = LNow();

	if (!strcmp(transId, TRANS_ID_PENDING) || !strcmp(transId, TRANS_ID_INVALID))
	{
	#if 20180330
		// 2018-11-26: FIX: must use physical key
		msql m(Name, "MAX(F_PAYMENT) AS F_MAX");

		m.Add(this, "F_TRANS_ID, F_TERM_ID");
		payment = dataBase->GetLong(str(m), "F_MAX");
	#else  // 20180330
		msql m;

		FloatCriteria(m, "MAX(F_PAYMENT)");
		TGetGroup g(str(m), new TLong("MAX", &payment));
	#endif  // 20180330
		if (++payment <= 0)
			generator_limit(Name);
	}
}

bool TPayment::Insert()
{
	Generate();
	return TGroup::Insert();
}

bool TPayment::operator<=(const TPayment &that) const
{
	int result = strcmp(transId, that.transId);
	return result ? result < 0 : accountType != that.accountType ? accountType > that.accountType : payment >= that.payment;
}

bool TPayment::SameTransact(const TPayment &next) const
{
	return !strcmp(next.transId, transId) && next.termId == termId &&
		!strcmp(ucn, next.ucn) && ucnType == next.ucnType &&
		date == next.date && next.time.Value() - time.Value() <= 1;
}

// ----- TOldFirm ------------------------------------------------------------
TOldFirm::TOldFirm()
	: TFirm("T_OLD_FIRM", TOldFirm::Clone)
{
	descs = Descs + TABLE_OLD_FIRM;
}

TGroup *TOldFirm::Clone()
{
	return new TOldFirm;
}

// ----- TExecListBase -------------------------------------------------------
TExecListBase::TExecListBase(const char *name, TGroup *(*const tClone)())
	: TGroup(name, tClone)
{
	Add(
		new TLong("F_EXEC_LIST_NO", &execListNo),
		new TLong("F_EXEC_LIST_YEAR", &execListYear),
		new TCharAlias("F_TYPE", &type),
		new TLong("F_EXEC_LIST_KIND", &execListKind),
		NULL
	);
}

void TExecListBase::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_EXEC_LIST_NO, F_EXEC_LIST_YEAR, F_TYPE, F_EXEC_LIST_KIND");
}

void TExecListBase::Criteria(TSubpoena *subpoena, msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(subpoena, "F_EXEC_LIST_NO");
	m.AddLong("F_EXEC_LIST_YEAR", subpoena->subpoenaDate.year);
	m.Add(subpoena, "F_TYPE, F_EXEC_LIST_KIND");
}

// ----- TExecListPerson -----------------------------------------------------
TExecListPerson::TExecListPerson()
	: TExecListBase("T_EXEC_LIST_PERSON", TExecListPerson::Clone), TTryAddress(NULL)
{
	Add(
		new TLong("F_SUM_NO", &sumNo),
		new TString("F_UCN", ucn, sizeof ucn),
		new TCharAlias("F_UCN_TYPE", &ucnType),
		new TCharAlias("F_INVOLVEMENT", &involvement),
		NULL
	);

	descs = Descs + TABLE_EXEC_LIST_PERSON;
}

TGroup *TExecListPerson::Clone()
{
	return new TExecListPerson;
}

void TExecListPerson::ExactCriteria(msql &m, const char *fields)
{
	TExecListPerson::FloatCriteria(m, fields);
	m.Add(this, "F_UCN, F_UCN_TYPE");
}

void TExecListPerson::FloatCriteria(msql &m, const char *fields)
{
	TExecListBase::ExactCriteria(m, fields);
	m.Add(this, "F_SUM_NO");
}

void TExecListPerson::Print(mstr &m)
{
	TSideWind sideWind;
	sideWind << this;
	sideWind.PrintNames(m);
}

void TExecListPerson::SetAddress(const TGroup *tAddress, bool foreign)
{
	delete address;
	address = TAnyAddress::NewGroup(foreign);
	*address << tAddress;
	triedAddress = true;
}

void TExecListPerson::SetAddress(TGArray<TExecListAddress> &addresses)
{
	for (int i = 0; i < addresses.Count(); i++)
	{
		TExecListAddress *address = addresses[i];

		if (!strcmp(address->ucn, ucn) && address->ucnType == ucnType)
		{
			SetAddress(address, any(address->country));
			break;
		}
	}
}

// ----- TExecListSumBase ----------------------------------------------------
TExecListSumBase::TExecListSumBase(const char *name, TGroup *(*const tClone)())
	: TExecListBase(name, tClone)
{
	Add(
		new TLong("F_SUM_NO", &sumNo),
		new TMoney("F_SUM", &sum),
		new TStringAlias("F_CURRENCY", currency, sizeof currency),
		new TString("F_SUM_TEXT", sumText, sizeof sumText),
		NULL
	);
}

void TExecListSumBase::ExactCriteria(msql &m, const char *fields)
{
	TExecListSumBase::FloatCriteria(m, fields);
	m.Add(this, "F_SUM_NO");
}

void TExecListSumBase::FloatCriteria(msql &m, const char *fields)
{
	TExecListBase::ExactCriteria(m, fields);
}

bool TExecListSumBase::Generate()
{
	msql m;
	FloatCriteria(m, "MAX(F_SUM_NO)");
	return generate_max(Name, str(m), &sumNo, SUM_NO_MAX, execListYear);
}

bool TExecListSumBase::Insert()
{
	return (sumNo || Generate()) && TExecListBase::Insert();
}

// ----- TExecListSum --------------------------------------------------------
TExecListSum::TExecListSum()
	: TExecListSumBase("T_EXEC_LIST_SUM", TExecListSum::Clone)
{
	Add(
		new TCharAlias("F_SUM_TYPE", &sumType),
		new TStringAlias("F_SENDER", sender, sizeof sender),
		NULL
	);

	descs = Descs + TABLE_EXEC_LIST_SUM;
}

TGroup *TExecListSum::Clone()
{
	return new TExecListSum;
}

bool TExecListSum::Save(bool edit)
{
	if (!TExecListSumBase::Save(edit))
		return false;

	if (edit)
	{
		msql m("T_EXEC_LIST_PERSON", MQ_DELETE);
		m.Add(this, "F_EXEC_LIST_NO, F_EXEC_LIST_YEAR, F_TYPE, F_EXEC_LIST_KIND, F_SUM_NO");
		dataBase->Execute(str(m), ET_ERROR);
	}

	for (int i = 0; i < persons.Count(); i++)
	{
		*persons[i] << this;
		persons[i]->Insert();
	}

	return true;
}

bool TExecListSum::SameList(const TExecListSum *sum)
{
	return execListNo == sum->execListNo && execListYear == sum->execListYear && type == sum->type &&
		execListKind == sum->execListKind;
}

bool TExecListSum::SamePersons(const TExecListSum *sum)
{
	if (persons.Count() != sum->persons.Count())
		return false;

	for (int i = 0; i < persons.Count(); i++)
	{
		TExecListPerson *person = persons[i];
		int n;

		for (n = 0; n < sum->persons.Count(); n++)
			if (sum->persons[n]->ucnType == person->ucnType && !strcmp(sum->persons[n]->ucn, person->ucn))
				break;

		if (n == sum->persons.Count())
			return false;
	}

	return true;
}

// ----- TExecList -----------------------------------------------------------
TExecList::TExecList()
	: TExecListBase("T_EXEC_LIST", TExecList::Clone), recipient(0)
{
	Add(
		new TDate("F_EXEC_LIST_DATE", &execListDate, false),
		new TLong("F_NO", &no),
		new TLong("F_YEAR", &year),
		NEW_RCDKEY("F_NIA_", &nia),
		new TStringAlias("F_TDEP", tdep, sizeof tdep),
		new TString("F_TDEP_IN_NO", tdepInNo, sizeof tdepInNo),
		new TDate("F_TDEP_DATE", &tdepDate, false),
		new TText("F_TDEP_TEXT", &tdepText, SIZE_OF_TDEP_TEXT),
		new TCharAlias("F_TDEP_UCN_TYPE", &tdepUCNType),
		NULL
	);

	descs = Descs + TABLE_EXEC_LIST;
}

bool TExecList::InsertSumPerson(TBaseWind *baseWind, int index)
{
	for (int i = 0; i < allSumPersons.Count(); i++)
	{
		TExecListPerson *person = allSumPersons[i];

		if (!strcmp(person->ucn, baseWind->ucn) && person->ucnType == baseWind->ucnType &&
			person->involvement == baseWind->involvement)
		{
			return false;
		}
	}

	TExecListPerson *person = new TExecListPerson;

	*person << this;
	strcpy(person->ucn, baseWind->ucn);
	person->ucnType = baseWind->ucnType;
	person->involvement = baseWind->involvement;
	allSumPersons.Insert(person, index);
	return true;
}

TGroup *TExecList::Clone()
{
	return new TExecList;
}

bool TExecList::CheckSend(TOutReg *outReg)
{
	TOutReg tOutReg;

	if (!outReg)
		outReg = &tOutReg;

	if (nia.no)
	{
		TNIAProtocol protocol;
		protocol.nia = nia;

		if (protocol.Get("F_OUT_NO, F_OUT_YEAR") && protocol.outNo)
		{
			outReg->kind = KIND_OUT_LETTER;
			outReg->key.no = protocol.outNo;
			outReg->key.year = protocol.outYear;
			outReg->Get("F_DATE");
		}

		return true;
	}

	TSubpoena subpoena;
	msql m("T_SUBPOENA S JOIN T_SUBPOENA L ON L.F_NO = S.F_NO AND L.F_YEAR = S.F_YEAR AND L.F_TYPE = S.F_TYPE "
		"AND L.F_KIND = S.F_KIND AND L.F_DATE = S.F_DATE AND L.F_EVENT_STATUS = S.F_EVENT_STATUS "
		"AND L.F_REGARD = S.F_SUBPOENA_NO", "L.F_OUTREG_NO, L.F_OUTREG_YEAR");

	m.AddLong("S.F_NO", no);
	m.AddLong("S.F_YEAR", year);
	m.AddChar("S.F_TYPE", type);
	m.AddLong("S.F_EXEC_LIST_NO", execListNo);
	m.AddLong("S.F_SUBPOENA_DATE", execListDate.Value());
	m.AddLong("S.F_EXEC_LIST_KIND", execListKind);
	m.AddIsNull("L.F_OUTREG_NO", false);

	TQuery q(str(m));
	if (q.Read())
	{
		subpoena << q;
		outReg->kind = KIND_OUT_LETTER;
		outReg->key.no = subpoena.outRegNo;
		outReg->key.year = subpoena.outRegYear;
		outReg->Get("F_DATE");
		return true;
	}

	return false;
}

bool TExecList::Delete(bool exact)
{
	bool result = TExecListBase::Delete(exact);

	if (exact && result)
		DeleteSumsPersons();

	return result;
}

void TExecList::DeleteSumsPersons()
{
	static constant tables[] = { "T_EXEC_LIST_SUM", "T_EXEC_LIST_PERSON", "T_EXEC_LIST_ADDRESS", NULL };

	for (constant *table = tables; *table; table++)
	{
		msql m(*table, MQ_DELETE);
		m.Add(this, "F_EXEC_LIST_NO, F_EXEC_LIST_YEAR, F_TYPE, F_EXEC_LIST_KIND");
		dataBase->Execute(str(m), ET_ERROR);
	}
}

void TExecList::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_EXEC_LIST_YEAR");

	if (EXEC_LIST_NUMBERING & EXEC_LIST_NUMBERING_EXEC_KIND)
		m.Add(this, "F_EXEC_LIST_KIND");

	if (EXEC_LIST_NUMBERING & EXEC_LIST_NUMBERING_TYPE)
		m.Add(this, "F_TYPE");
}

bool TExecList::Generate()
{
	msql m;
	FloatCriteria(m, "MAX(F_EXEC_LIST_NO)");

	return generate_max(Name, str(m), &execListNo, XFER_NO_MAX, strchr(Default->functions, RIGHT_ANTILAWNO) ? 0 :
		execListYear);
}

bool TExecList::Insert()
{
	return (execListNo || Generate()) && TExecListBase::Insert();
}

bool TExecList::Save(bool edit)
{
	if (!TExecListBase::Save(edit))
		return false;

	if (edit)
		DeleteSumsPersons();

	TGArray<TExecListAddress> addresses;

	for (int i = 0; i < sums.Count(); )
	{
		TExecListSum *sum = sums[i];

		*sum << this;
		sum->sumNo = ++i;
		sum->Save(false);		// deleted above

		// 2015:075 save addresses
		for (int i = 0; i < sum->persons.Count(); i++)
		{
			TExecListPerson *person = sum->persons[i];

			if (person->address)
			{
				int n;

				for (n = 0; n < addresses.Count(); n++)
					if (!strcmp(addresses[n]->ucn, person->ucn) && addresses[n]->ucnType == person->ucnType)
						break;

				if (n < addresses.Count())
					continue;

				TExecListAddress *address = new TExecListAddress;

				*address << this;
				*address << person->address;
				address->Insert();
				addresses.Add(address);
			}
		}
	}

	return true;
}

bool TExecList::operator<=(const TExecListBase &that)
{
	return execListYear != that.execListYear ? execListYear < that.execListYear :
		type != that.type ? type < that.type :
		execListNo != that.execListNo ? execListNo < that.execListNo :
		execListKind <= that.execListKind;
}

// ----- TNIAProtocol --------------------------------------------------------
TNIAProtocol::TNIAProtocol()
	: TGroup("T_NIA_PROTOCOL", TNIAProtocol::Clone)
{
	Add(
		NEW_RCDKEY("F_NIA_", &nia),
		new TStringAlias("F_NIA_TDEP", niaTDep, sizeof niaTDep),
		new TLong("F_OUT_NO", &outNo),
		new TLong("F_OUT_YEAR", &outYear),
		NULL
	);

	descs = Descs + TABLE_NIA_PROTOCOL;
}

TGroup *TNIAProtocol::Clone()
{
	return new TNIAProtocol;
}

void TNIAProtocol::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_NIA_NO, F_NIA_YEAR, F_NIA_TYPE");
}

void TNIAProtocol::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_NIA_YEAR, F_NIA_TYPE");
}

bool TNIAProtocol::Generate()
{
	msql m;
	FloatCriteria(m, "MAX(F_NIA_NO)");
	return generate_max(Name, str(m), &nia.no, NO_MAX, strchr(Default->functions, RIGHT_ANTILAWNO) ? 0 : nia.year);
}

bool TNIAProtocol::Insert()
{
	return (nia.no || Generate()) && TGroup::Insert();
}

// ----- TExecListCollect ----------------------------------------------------
TExecListCollect::TExecListCollect()
	: TExecListSumBase("T_EXEC_LIST_COLLECT", TExecListCollect::Clone)
{
	Add(
		new TDate("F_DATE", &date, false),
		new TLong("F_FLAGS", &flags),
		new TCheck("F_FLAGS_VOLUNTARY", &flags, EXEC_LIST_COLLECT_VOLUNTARY),
		new TString("F_UCN", ucn, sizeof ucn),
		new TCharAlias("F_UCN_TYPE", &ucnType),
		NULL
	);

	descs = Descs + TABLE_EXEC_LIST_COLLECT;
}

TGroup *TExecListCollect::Clone()
{
	return new TExecListCollect;
}

// ----- TExecListAddress ----------------------------------------------------
TExecListAddress::TExecListAddress()
	: TExecListBase("T_EXEC_LIST_ADDRESS", TExecListAddress::Clone)
{
	Add(
		new TString("F_UCN", ucn, sizeof ucn),
		new TCharAlias("F_UCN_TYPE", &ucnType),
		new TLongAlias("F_UCLP", &uclp),
		new TLongAlias("F_STREET", &street),
		new TString("F_ADR_NO", adrNo, sizeof adrNo),
		new TString("F_SUB_NO", subNo, sizeof subNo),
		new TString("F_ENTRANCE", entrance, sizeof entrance),
		new TString("F_FLOOR", floor, sizeof floor),
		new TString("F_APARTMENT", apartment, sizeof apartment),
		new TString("F_BLOCK_NO", blockNo, sizeof blockNo),
		new TString("F_PHONE", phone, sizeof phone),
		new TString("F_TELEX", telex, sizeof telex),
		new TString("F_FAX", fax, sizeof fax),
		new TString("F_EMAIL", email, sizeof email),
		new TString("F_REMARK", remark, sizeof remark),
		new TStringAlias("F_COUNTRY", country, sizeof country),
		new TString("F_FOREIGN", foreign, sizeof foreign),
		NULL
	);

	descs = Descs + TABLE_EXEC_LIST_ADDRESS;
}

TGroup *TExecListAddress::Clone()
{
	return new TExecListAddress;
}

void TExecListAddress::ExactCriteria(msql &m, const char *fields)
{
	TExecListAddress::FloatCriteria(m, fields);
	m.Add(this, "F_UCN, F_UCN_TYPE");
}

void TExecListAddress::FloatCriteria(msql &m, const char *fields)
{
	TExecListBase::ExactCriteria(m, fields);
}

// ----- TJurorLog -----------------------------------------------------------
TJurorLog::TJurorLog()
	: TGroup("T_JUROR_LOG", TJurorLog::Clone)
{
	Add(
		new TLong("F_JUROR_LOG", &jurorLog),
		NEW_RCDKEY("F_", &key),
		new TCharAlias("F_KIND", &kind),
		new TDate("F_DATE", &date, false),
		new TString("F_UCN", ucn, sizeof ucn),
		new TChar("F_OPER", &oper),
		new TPersonAlias("F_JUROR", juror, sizeof juror),
		new TLong("F_INDEX", &index),
		new TTimeStamp("F_STAMP", &stamp),
		new TString("F_TEXT", text, sizeof text),
		new TZLong("F_EXCESS", &excess),
		new TLong("F_RES_1", resBlock + 1),
		new TLong("F_RES_2", resBlock + 3),
		new TLong("F_RES_3", resBlock + 2),
		new TLong("F_RES_4", resBlock + 0),
		NULL
	);

	descs = Descs + TABLE_JUROR_LOG;
}

void TJurorLog::CalcMD5(long *tResBlock)
{
	char buffer[128];
	char *ptr = buffer + 8;
	char *s;

	for (s = buffer; s < buffer + sizeof buffer; s += sizeof(int))
		*(int *) s = rand();

	memcpy(ptr, &jurorLog, sizeof jurorLog);
	ptr += sizeof jurorLog;

	long pKey[2] = { key.no, (key.year << (11 + excess)) + key.type };
	memcpy(ptr, pKey, sizeof pKey);
	ptr += sizeof pKey;

	char pStamp[6] = { (char) (stamp.year - YEAR_MIN), (char) stamp.month, (char) stamp.day, (char) stamp.hour,
		(char) stamp.minute, (char) stamp.second };
	memcpy(ptr, pStamp, sizeof pStamp);
	ptr += sizeof pStamp;

	char pDate[3] = { (char) (date.year - YEAR_MIN), (char) date.month, (char) date.year };
	memcpy(ptr, pDate, sizeof pDate);
	ptr += sizeof pDate;

	*ptr++ = kind;

	*(int *) ptr = atoi(ucn);
	ptr += sizeof(int);

	*(int *) ptr = atoi(juror);
	ptr += sizeof(int);

	*ptr++ = oper;

	// FiX! was strcpy(ptr, text): buffer overflow
	size_t remaining = sizeof buffer - (ptr - buffer);
	size_t text_len = strlen(text);
	char buffer2[384];
	char *ptr2 = buffer2 + (ptr - buffer);

	memcpy(buffer2, buffer, sizeof buffer);
	strcpy(ptr2, text);
	ptr2 += text_len;

	if (text_len >= remaining)
	{
		memcpy(ptr, text, remaining);
		ptr += remaining;
	}
	else
	{
		strcpy(ptr, text);
		ptr += text_len;
	}

	unsigned seed = 0;

	for (s = buffer2 + 8; s < ptr2; s++)
		seed ^= *s << (8 * ((s - buffer2) & 0x03));

	srand(seed);
	*(int *) buffer = rand();
	*(int *) (buffer + 4) = rand();

	while (ptr < buffer + sizeof buffer)
		*ptr++ = (char) (rand() & 0xFF);

	calc_md5sum(buffer, sizeof buffer, (char *) tResBlock);
}

void TJurorLog::CheckMD5()
{
	long tResBlock[4];

	CalcMD5(tResBlock);
	if (memcmp(tResBlock, resBlock, sizeof resBlock))
		fatal("%s: невалидна контролна сума.", Name);
}

TGroup *TJurorLog::Clone()
{
	return new TJurorLog();
}

void TJurorLog::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_NO, F_YEAR, F_TYPE, F_JUROR_LOG, F_EXCESS");
}

void TJurorLog::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_NO, F_YEAR, F_TYPE, F_EXCESS");
	m.AddOrder("F_JUROR_LOG");
}

bool TJurorLog::Insert()
{
	strimall(text);
	CalcMD5(resBlock);
	return TGroup::Insert();
}

// ----- TAttachStat ---------------------------------------------------------
TAttachStat::TAttachStat()
	: TRCDKeyGroup("T_ATTACH_STAT", TAttachStat::Clone), found(false)
{
	Add(
		new TString("F_UCN", ucn, sizeof ucn),
		new TDate("F_FROM", &from, true),
		new TCharAlias("F_KIND", &kind),
		new TDate("F_DATE", &date, true),
		NULL
	);

	descs = Descs + TABLE_ATTACH_STAT;
	strcpy(ucn, Default->ucn);
}

TGroup *TAttachStat::Clone()
{
	return new TAttachStat;
}

void TAttachStat::ExactCriteria(msql &m, const char *fields)
{
	FloatCriteria(m, fields);
	m.Add(this, "F_UCN, F_FROM, F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE");
}

void TAttachStat::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_UCN, F_FROM");
}

// ----- TRegisterAct --------------------------------------------------------
TRegisterAct::TRegisterAct()
	: TRCDKeyContainer("T_REGISTER_ACT", TRegisterAct::Clone)
{
	Add(
		new TPersonAlias("F_JUDGE", judge, sizeof judge),
		new TLong("F_ACT_NO", &actNo),
		new TLong("F_TEXT_MTIME", mtime + TYPE_TEXT),
		new TBLOB("F_TEXT_DATA", fileData + TYPE_TEXT),
		new TLong("F_IMAGE_MTIME", mtime + TYPE_IMAGE),
		new TBLOB("F_IMAGE_DATA", fileData + TYPE_IMAGE),
		new TCharAlias("F_TARGET_KIND", &targetKind),
		NEW_RCDKEY("F_TARGET_", &target),
		new TDate("F_TARGET_DATE", &targetDate, false),
		new TTimeStamp("F_TARGET_STAMP", &targetStamp),
		new TString("F_TEXT", text, sizeof text),
		NULL
	);

	dataBase = ExtraDataBase;
	descs = Descs + TABLE_REGISTER_ACT;
	strcpy(judge, Default->ucn);
}

TGroup *TRegisterAct::Clone()
{
	return new TRegisterAct;
}

void TRegisterAct::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_JUDGE, F_ACT_NO");
}

void TRegisterAct::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_DATE, F_JUDGE");

	if (kind)
		m.Add(this, "F_KIND");
}

bool TRegisterAct::Insert()
{
	msql m(Name, "MAX(F_ACT_NO) AS F_MAX");

	m.Add(this, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_JUDGE");
	actNo = dataBase->GetLong(str(m), "F_MAX");

	if (++actNo > REGISTER_ACT_MAX)
		generator_limit(Name);

	return TRCDKeyGroup::Insert();
}

void TRegisterAct::Print(mstr &m)
{
	Type->Seek(key.type);
	m.printf("%-5s  %5ld/%ld", Type->alias, key.no, key.year);
}

void TRegisterAct::ViewFiles(TWindow *parent)
{
	if (Get("F_TEXT_DATA, F_IMAGE_DATA"))
	{
		for (int type = 0; type < TROUBLE_COUNT; type++)
		{
			if (!fileData[type].size)
				continue;

			char name[SIZE_OF_PATH];

			if (attach_write_lock(this, name, type, fileData[type], mtime[type]))
			{
				attach_view(parent, name, type);
				add_unlink_file(basename(name), false, true);
			}

			mbk_clear(fileData + type);
		}
	}
}

bool TRegisterAct::FetchFiles(TElectricTrouble *trouble)
{
	bool result = Get("F_TEXT_DATA, F_IMAGE_DATA");

	if (result)
	{
		for (int type = 0; type < TROUBLE_COUNT; type++)
		{
			if (!fileData[type].size)
				continue;

			char name[SIZE_OF_PATH];

			if (attach_write_lock(this, name, type, fileData[type], mtime[type]))
			{
				trouble->name[type]->SetS(name);
				add_unlink_file(basename(name), false, true);
			}
			else
			{
				result = false;
				break;
			}
		}
	}

	return result;
}

bool TRegisterAct::ReWriteFiles()
{
	for (int type = 0; type < TROUBLE_COUNT; type++)
	{
		if (!fileData[type].size)
			continue;

		char name[SIZE_OF_PATH];

		if (!attach_write_lock(this, name, type, fileData[type], mtime[type]))
			return false;
	}

	return true;
}

// ----- TDeliverVisit -------------------------------------------------------
TDeliverVisit::TDeliverVisit(const char *name, TGroup *(*const tClone)())
	: TGroup(name, tClone)
{
	Add(
		new TTimeStamp("F_STAMP", &stamp),
		new TCoord("F_COORD_N", &coordN),
		new TCoord("F_COORD_E", &coordE),
		NULL
	);
}

void TDeliverVisit::Print(mstr &m)
{
	mstr text;

	text.printf("Посещение от %D %lT", &(const CDate &) stamp, &(const CTime &) stamp);
	print_gps_link(m, coordN, coordE, str(text));
}

void TDeliverVisit::ExactCriteria(msql &m, const char *fields)
{
	FloatCriteria(m, fields);
	m.Add(this, "F_STAMP");
}

void TDeliverVisit::PrintVisits(mstr &m, TGroup *parent)
{
	msql w;

	*this << parent;
	FloatCriteria(w);

	TQuery q(str(w));
	while (q.Read())
	{
		*this << q;
		Print(m);
	}
}

// ----- TSubpoenaVisit ------------------------------------------------------
TSubpoenaVisit::TSubpoenaVisit()
	: TDeliverVisit("T_SUBPOENA_VISIT", TSubpoenaVisit::Clone)
{
	Add(
		NEW_RCDKEY("F_", &key),
		new TCharAlias("F_KIND", &kind),
		new TDate("F_DATE", &date, true),
		new TLong("F_SUBPOENA_NO", &subpoenaNo),
		NULL
	);

	descs = Descs + TABLE_SUBPOENA_VISIT;
}

TGroup *TSubpoenaVisit::Clone()
{
	return new TSubpoenaVisit;
}

void TSubpoenaVisit::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_SUBPOENA_NO");
}

// ----- TAnnounceVisit ------------------------------------------------------
TAnnounceVisit::TAnnounceVisit()
	: TDeliverVisit("T_ANNOUNCE_VISIT", TAnnounceVisit::Clone)
{
	Add(
		new TLong("F_NO", &key.no),
		new TLong("F_YEAR", &key.year),
		new TCharAlias("F_KIND", &key.kind),
		new TLongAlias("F_COURT", &court),
		new TLong("F_ANNOUNCE", &announce),
		NULL
	);

	descs = Descs + TABLE_ANNOUNCE_VISIT;
}

TGroup *TAnnounceVisit::Clone()
{
	return new TAnnounceVisit;
}

void TAnnounceVisit::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_NO, F_YEAR, F_KIND, F_COURT, F_ANNOUNCE");
}

// ----- TElectric -----------------------------------------------------------
TElectric::TElectric()
	: TRCDKeyContainer("T_ELECTRIC", TElectric::Clone)
{
	Add(
		new TLong("F_EXTRA_NO", &extraNo),
		new TString("F_DESCRIPT", descript, sizeof descript),
		new TString("F_EXTRA_NAME", extraName, sizeof extraName),
		new TLong("F_EXTRA_TYPE", &extraType),
		new TString("F_UCN", ucn, sizeof ucn),
		new TCharAlias("F_UCN_TYPE", &ucnType),
		new TCharAlias("F_INVOLVEMENT", &involvement),
		new TCharAlias("F_SUBPOENA_KIND", &subpoenaKind),
		new TCharAlias("F_EVENT_STATUS", &eventStatus),
		NULL
	);

	descs = Descs + TABLE_ELECTRIC;
}

TGroup *TElectric::Clone()
{
	return new TElectric;
}

bool TElectric::Delete(bool exact)
{
	return TRCDKeyGroup::Delete(exact);
}

bool TElectric::ElectricDelete(const TLawsuit *lawsuit, const TRCDKeyContainer *container)
{
	if (unlock_delete_file(extraName, true))
	{
		electric_log(LOG_DELETE, this, extraName, extraNo);
		eportal_delete(this, lawsuit, container);

		if (extraType == TYPE_HTML)
			html_group_action(extraName, unlock_delete_file, VOID_TRUE);

		if (ExtraUser())
			Delete(true);

		return true;
	}

	return false;
}

bool TElectric::ExtraSurrouend(long extraNo)
{
	return extraNo > SUBPOENA_NO_MAX + EXTRA_NO_MAX && extraNo <= SUBPOENA_NO_MAX + EXTRA_NO_MAX + SURROUNDMENTS_MAX;
}

void TElectric::ExactCriteria(msql &m, const char *fields)
{
	TRCDKeyContainer::ExactCriteria(m, fields);
	m.Add(this, "F_EXTRA_NO");
}

void TElectric::FloatCriteria(msql &m, const char *fields)
{
	TRCDKeyContainer::ExactCriteria(m, fields);
}

bool TElectric::Insert()
{
	msql m(Name, "MAX(F_EXTRA_NO) AS F_MAX");

	m.Add(this, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE");
	extraNo = dataBase->GetLong(str(m), "F_MAX");

	if (!extraNo)
		extraNo = SUBPOENA_NO_MAX + 1;
	else if (!ExtraUser(++extraNo))
		generator_limit(Name);

	return TRCDKeyGroup::Insert();
}

void TElectric::Print(mstr &m)
{
	switch (extraNo)
	{
		case 0 :
		{
			TString *extraText = FindExtraText();

			if (strchr(KIND_ANY_OTHER_DOCUMENTS, kind) && extraText && *extraText->S())
				m.cat(extraText->S());
			else
				m.cat(Kind->Seek(kind));
			break;
		}
		case EXTRA_NO_XFER_MOTIVES : m.cat("Мотиви"); break;
		case EXTRA_NO_SUBPLIST_FIXED :
		case EXTRA_NO_SUBPLIST_OTHER :
		{
			m.printf("Списък на %s лица", extraNo == EXTRA_NO_SUBPLIST_FIXED ? "призованите" : "уведомените");
			break;
		}
	#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
		case EXTRA_NO_PROTECTIVE : m.cat(Kind->Seek(KIND_PROTECTIVE)); break;
	#endif  // AREAL || REGIONAL
	#if COURT_TYPE == COURT_REGIONAL
		case EXTRA_NO_IMMED_PROTECT : m.cat(Kind->Seek(KIND_IMMED_PROTECT)); break;
	#endif  // REGIONAL
		case EXTRA_NO_OUTREG_SUP_APP : m.cat("Приложение за ВКС"); break;
		default :
		{
		#if COURT_TYPE == COURT_REGIONAL
			if (extraNo >= EXTRA_NO_EXECUTIVE_BASE && extraNo <= EXTRA_NO_EXECUTIVE_LAST)
				m.cat(Kind->Seek(KIND_EXECUTIVE));
			else
		#endif  // REGIONAL
			m.cat(descript);	// user, subpoena
		}
	}
}

bool TElectric::Update(const char *fields)
{
	return TRCDKeyGroup::Update(fields);
}

bool TElectric::operator<=(const TElectric &that) const
{
#if 20160259
	int prior = ExtraSubpoena() ? 2 : ExtraUser() ? 1 : 0;
	int thatPrior = that.ExtraSubpoena() ? 2 : that.ExtraUser() ? 1 : 0;

	return prior != thatPrior ? prior < thatPrior : extraNo != that.extraNo ? extraNo < that.extraNo :
		extraType <= that.extraType;
#else  // 20160259
	if (extraNo != that.extraNo)
	{
		bool extraSubpoena = ExtraSubpoena();
		bool thatExtraSubpoena = that.ExtraSubpoena();

		if (extraSubpoena && thatExtraSubpoena)
		{
			int result = strcmp(descript, that.descript);

			if (result)
				return result < 0;
		}

		if (extraSubpoena != thatExtraSubpoena)
			return !extraSubpoena;

		return extraNo <= that.extraNo;
	}

	return extraType <= that.extraType;
#endif  // 20160259
}

TKnownNamesPerson::TKnownNamesPerson()
	: TCitizenNames(this)
{
}

void TKnownNamesPerson::NamesCriteria(msql &m)
{
	m.Begin("T_KNOWN_PERSON K LEFT JOIN T_CITIZEN C ON C.F_UCN = K.F_UCN AND C.F_UCN_TYPE = K.F_UCN_TYPE",
		"K.*, C.F_NAME, C.F_RENAME, C.F_FAMILY, C.F_REFAMILY");
	m.Add(this, "F_INVOLVEMENT");
	m.AddOrder("K.F_UCN_TYPE DESC, K.F_UCN");
}

void TKnownNamesPerson::Print(mstr &m)
{
	if (!any(cache))
	{
		if (strchr(UCN_CITIZENS, ucnType))
			print_user(cache, this, !strchr(INVOLVEMENT_CSJIDS, involvement), true);
		else
			TKnownPerson::Print(cache);
	}

	m.cat(cache);
}

TEPortAssign::TEPortAssign()
	: TRCDKeyGroup("T_EPORT_ASSIGN", TEPortAssign::Clone)
{
	Add(
		NEW_RCDKEY("F_LAWSUIT_", &lawsuit),
		new TCharAlias("F_INVOLVEMENT", &involvement),
		new TLong("F_FLAGS", &flags),
		new TCheck("F_EPORTAL_SUBPOENA", &flags, EPORTAL_SUBPOENA),
		NULL
	);

	descs = Descs + TABLE_EPORT_ASSIGN;
}

TGroup *TEPortAssign::Clone()
{
	return new TEPortAssign;
}

void TEPortAssign::ExactCriteria(msql &m, const char *fields)
{
	TRCDKeyGroup::ExactCriteria(m, fields);
	m.Add(this, "F_LAWSUIT_NO, F_LAWSUIT_YEAR, F_LAWSUIT_TYPE, F_INVOLVEMENT");
}

void TEPortAssign::FloatCriteria(msql &m, const char *fields)
{
	TRCDKeyGroup::ExactCriteria(m, fields);
}

// ----- TExternSession ------------------------------------------------------
TExternSession::TExternSession()
	: TBaseSession("T_EXTERN_SESSION", TExternSession::Clone)
{
	descs = Descs + TABLE_EXTERN_SESSION;
}

TGroup *TExternSession::Clone()
{
	return new TExternSession;
}

bool TExternSession::Delete(bool exact)
{
	if (exact)
	{
		TSessionTimes sessionTimes;
		sessionTimes << this;
		sessionTimes.Delete(false);
	}

	return TBaseSession::Delete(exact);
}

long TExternSession::Excess() const
{
	return 1;
}

char TExternSession::IndexType() const
{
	return TYPE_NULL;
}

char TExternSession::IndexKind() const
{
	return KIND_NULL;
}

bool TExternSession::Insert()
{
	return TRCDKeyGroup::Insert();
}

void TExternSession::Signal(int, const void *)
{
}

// ----- TExternLawsuit ------------------------------------------------------
TExternLawsuit::TExternLawsuit()
	: TExternContainer("T_EXTERN_LAWSUIT", TExternLawsuit::Clone)
{
	Add(
		new TDate("F_FORCE_DATE", &forceDate, false),
		new TLong("F_SOURCE_KIND", &sourceKind),
		NULL
	);

	descs = Descs + TABLE_EXTERN_LAWSUIT;
}

TGroup *TExternLawsuit::Clone()
{
	return new TExternLawsuit;
}

long TExternLawsuit::Excess() const
{
	return 1;
}

bool TExternLawsuit::Insert()
{
	return TRCDKeyGroup::Insert();
}

void TExternLawsuit::Involvements(mstr &m) const
{
	if (sourceKind)
		kind2involvements((char) sourceKind, m);

	TExternContainer::Involvements(m);
}

char TExternLawsuit::LawKind() const
{
	return kind;
}

void TExternLawsuit::Signal(int, const void *)
{
}

// ----- TExternSideWind -----------------------------------------------------
TExternSideWind::TExternSideWind()
	: TBaseWind("T_EXTERN_SIDE_WIND", TExternSideWind::Clone)
{
	Add(new TString("F_EXTERN_ID", externId, sizeof externId));
	descs = Descs + TABLE_EXTERN_SIDE_WIND;
}

TGroup *TExternSideWind::Clone()
{
	return new TExternSideWind;
}

long TExternSideWind::Excess() const
{
	return 1;
}
