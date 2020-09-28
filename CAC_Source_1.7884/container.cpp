#include "cac.h"

// ----- TRCDKeyContainer ----------------------------------------------------
TRCDKeyContainer::TRCDKeyContainer(const char *name, TGroup *(*const tClone)())
	: TRCDKeyGroup(name, tClone), triedStamp(false)
{
	Add(
		new TCharAlias("F_KIND", &kind),
		new TDate("F_DATE", &date, true),
		new TDateTime("F_STAMP", &stamp),
		NULL
	);

	kind = Default->kind;
}

char TRCDKeyContainer::Category(char involvement)
{
	char category;

	if (Suitor(involvement))
		category = SIDEWIND_LEFT;
	else if (Other(involvement))
		category = SIDEWIND_THIRD;
	else
		category = SIDEWIND_RIGHT;

	return category;
}

bool TRCDKeyContainer::Combine(TRCDKeyContainer *source)
{
	// 2004:006 BRW: copied from TDatedContainer
	if (!TGroup::Insert())
		return false;

	Signal(ELECTRIC_ADD);

	if (source)
	{
		TSideWind sideWind;
		TLinkWind linkWind;

		sideWind.Transfer(source, this, NULL);
		linkWind.Transfer(source, this, NULL);
	}

	return true;
}

bool TRCDKeyContainer::Delete(bool exact)
{
	bool deleted = TRCDKeyGroup::Delete(exact);

	if (exact && deleted)
		Signal(ELECTRIC_DELETE);

	return deleted;
}

bool TRCDKeyContainer::Insert()
{
	if (!key.type)
		key.type = kind2type(kind);

	// 2004:236 LPR: regenerate; 2004:369 LPR: quickfix: ignore INDOC
	// 2015:278 LPR: permanent fix: request generate
	if (!Generate())
		return false;

	// 2004:006 BRW: copied from TDatedContainer
	TDummyContainer source;
	source << this;
	// Generic: search for same document (not event!) as itself
	return Combine(Latest(&source) ? &source : NULL);
}

void TRCDKeyContainer::Involvements(mstr &m) const
{
	kind2involvements(kind, m);
}

int TRCDKeyContainer::Kompare(const TRCDKeyContainer &that) const
{
	return kind != that.kind ? kind - that.kind :
		key.year != that.key.year ? key.year - that.key.year : key.no - that.key.no;
}

bool TRCDKeyContainer::Latest(TRCDKeyContainer *container)
{
	TSideWind sideWind;
	msql m(sideWind.Name, "DISTINCT F_KIND, F_DATE");

	nearest_event_crit(m, container, false, false);
	sideWind.key = container->key;

	TRCDKeyContainer *const tContainer = nearest_event_find(m, &sideWind, false);

	if (tContainer)
	{
		TVirtPtr VP(tContainer);
		*container << tContainer;
		return true;
	}

	return false;
}

#if COURT_TYPE == COURT_APPEAL
bool TRCDKeyContainer::Other(char kind, char involvement)
{
	if ((kind == KIND_1ST_PRIVATE_PUNISHMENT_LAW || kind == KIND_LEGAL_AID_REQUEST) && involvement == INVOLVEMENT_WITNESS)
		return false;
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
bool TRCDKeyContainer::Other(char, char involvement)
{
#endif  // ADMIN || AREAL || MILITARY || REGIONAL
	// 2009:131 LPR: rewritten with working (internal) ctype
	return
		!strchr(INVOLVEMENT_PSEUDO_LOWERS, involvement) &&
		(isupper(involvement) || isdigit(involvement) ||
		strchr(INVOLVEMENT_PSEUDO_UPPERS, involvement));
}

void TRCDKeyContainer::Signal(int sigtype, const void *data)
{
	// 2013:325 LPR: before log
	if (sigtype == ELECTRIC_ADD)
	{
		if (!TimeStamp())
		{
			((CDate &) stamp) = Today;
			((CTime &) stamp) = LNow();
		}

		triedStamp = true;
	}

	electric_signal(sigtype, this, data);
}

bool TRCDKeyContainer::Suitor(char involvement)
{
	// 2012:079 LPR: _surround + _other patch, making _other left/third
	return (key.type == TYPE_SURROUND && involvement == INVOLVEMENT_OTHER) || is_suitor_ll(kind, involvement);
}

bool TRCDKeyContainer::Update(const char *fields)
{
	if (TRCDKeyGroup::Update(fields))
	{
		// 2017:048 TRQ: FIX: support partial updates
		Signal(ELECTRIC_EDIT, fields);
		return true;
	}

	return false;
}

bool TRCDKeyContainer::operator<=(const TRCDKeyContainer &that) const
{
	if (date != that.date)
		return date < that.date;

	if (date >= FULL_KIND_DATE)
	{
		TryStamp();
		that.TryStamp();

		if (!stamp.Empty() && !that.stamp.Empty() && stamp != that.stamp)
			return stamp < that.stamp;
	}

	return strchr(KIND_RCDKEY_COMPARES, kind) <= strchr(KIND_RCDKEY_COMPARES, that.kind);
}

#if TESTVER
void TRCDKeyContainer::SetStamp(const TRCDKeyContainer *parent) const
{
	stamp = parent->stamp;
	triedStamp = parent->triedStamp;
}
#endif  // TESTVER

bool TRCDKeyContainer::TimeStamp() const
{
	TTime *time = (TTime *) Find("F_TIME");

	if (time && !time->Time().Empty())
	{
		((CDate &) stamp) = date;
		((CTime &) stamp) = time->Time();
		return true;
	}

	return false;
}

void TRCDKeyContainer::TryStamp() const
{
	if (!triedStamp)
	{
		if (!TimeStamp() && descs)
		{
			TLog log;
			TDecisionRider *rider = strchr(KIND_ENDOC_RIDERS, kind) ? (TDecisionRider *) this : NULL;

			if (rider)
			{
				rider->decision->TryStamp();
				stamp = rider->decision->stamp;
			}
			else
			{
				msql m(log.Name, "MAX(F_STAMP) AS F_STAMP");	// 2018-09-17: always MAX()

				m.Add(this, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE");
				m.AddChars("F_GOP", key.type == TYPE_OUTREG ? GOP_SEND_RECV : GOP_NEW_ENTER);

				TQuery q(str(m));
				if (q.Read())
				{
					log << q;
					stamp = log.stamp;
					stamp.second = 0;   // F_TIME lacks seconds
				}
			}
		}

		triedStamp = true;
	}
}

#if TESTVER
static bool are_parent_child(const TRCDKeyContainer *parent, const TRCDKeyContainer *container)
{
	if (strchr(KIND_REQUEST_RESOLUTIONS, container->kind))
	{
		return parent->key == container->key && strchr(KIND_INDOCS, parent->kind);
	}
	else if (strchr(KIND_PROVE_ACTIONS, container->kind))
	{
		return parent->key == container->key && strchr(KIND_PROVES, parent->kind);
	}
	else if (strchr(KIND_LAWSUITS, container->kind))
	{
		return (parent->key == ((const TLawsuit *) container)->source) && (strchr(KIND_INDOCS, parent->kind) ||
			(strchr(KIND_REQUEST_RESOLUTIONS, parent->kind) && parent->Status() == RESOLUTION_LAWED));
	}
	else if (strchr(KIND_SURROUNDS, container->kind))
	{
		return parent->key == ((const TSurround *) container)->lawsuit &&
			(strchr(KIND_LAWSUITS, parent->kind) || strchr(KIND_INDOCS, parent->kind));
	}
	else if (strchr(KIND_SESSIONS, container->kind))
	{
		return parent->key == container->key && strchr(KIND_LAWSUITS, parent->kind);
	}
	else if (strchr(KIND_ENDOCS, container->kind))
	{
		return parent->key == container->key && parent->date == container->date &&
			parent->kind == ((const TDecision *) container)->sessionKind;
	}
	else if (strchr(KIND_IN_REG_DOCS, container->kind))
	{
		const TInReg *appeal = (const TInReg *) container;

		if (strchr(KIND_ENDOCS, parent->kind))
		{
			return parent->key == appeal->lawsuit && parent->kind == appeal->decisionKind &&
				parent->date == appeal->decisionDate;
		}
		else if (strchr(KIND_SESSIONS, parent->kind))
		{
			return parent->key == appeal->lawsuit && parent->kind == appeal->sessionKind &&
				parent->date == appeal->decisionDate;
		}
		else if (strchr(KIND_INDOCS, parent->kind) || strchr(KIND_LAWSUITS, parent->kind))
		{
			return parent->key == appeal->lawsuit;
		}
		else if (strchr(KIND_IN_REG_DOCS, parent->kind))
		{
			return parent->key == appeal->decision;
		}
		else if (strchr(KIND_SURROUNDS, parent->kind))
		{
			return parent->key.no == appeal->surroundNo && parent->key.year == appeal->surroundYear;
		}
	}
	else if (strchr(KIND_OUT_REG_DOCS, container->kind))
	{
		const TOutReg *outreg = (const TOutReg *) container;

		if (strchr(KIND_INREG_EVENTS, container->kind))
		{
			const TInRegEvent *inevent = (const TInRegEvent *) parent;

			return inevent->outNo == outreg->key.no && inevent->outYear == outreg->key.year;
		}
		else if (strchr(KIND_IN_REG_DOCS, parent->kind))
		{
			return parent->key.no == outreg->inNo && parent->key.year == outreg->inYear;
		}
		else if (strchr(KIND_INDOCS, parent->kind) || strchr(KIND_LAWSUITS, parent->kind))
		{
			return parent->key == outreg->lawsuit;
		}
	}

	return false;
}

int TGSArray<TRCDKeyContainer>::Add(const TRCDKeyContainer *container)
{
	int result = TGroupArray::Add(container);
	// assuming mostly correct ordering
	bool reorder = false;

	for (int i = Count() - 1; i > result; i--)
	{
		const TRCDKeyContainer *parent = (*this)[i];

		if (are_parent_child(parent, container))
		{
			container->SetStamp(parent);
			reorder = true;
		}
	}

	if (reorder)
	{
		Detach(result);
		result = TGroupArray::Add(container);
	}

	return result;
}
#endif  // TESTVER

// ----- TExternContainer ----------------------------------------------------
TExternContainer::TExternContainer(const char *name, TGroup *(*const tClone)())
	: TRCDKeyContainer(name, tClone)
{
	Add(
		new TLongAlias("F_SUBJECT", &subject),
		new TQuick("F_QUICK", &quick),
		NULL
	);
}

const CDate &TExternContainer::ForceDate() const
{
	TDate *forceDate = TYPECHECK_PTR_CAST(TDate, Find("F_FORCE_DATE"));

	cac_assert(forceDate != NULL);
	return forceDate->Date();
}

// ----- TOriginContainer ----------------------------------------------------
TOriginContainer::TOriginContainer(const char *name, TGroup *(*const tClone)())
	: TExternContainer(name, tClone)
{
	Add(
		new TString("F_EISPP_NMR", eispp_nmr, sizeof eispp_nmr),
		new TCharAlias("F_OLD_KIND", &oldKind),
		new TCharAlias("F_COLLEGE", &college),
		new TLongAlias("F_OLD_SUBJECT", &oldSubject),
		NULL
	);
}

long TOriginContainer::Excess() const
{
	return 0;
}

char TOriginContainer::MainConnectType() const
{
	TConnect connect;
	msql m(connect.Name, "F_CONNECT_TYPE");

	m.Add(this, "F_NO, F_YEAR, F_TYPE");
	m.AddChars("F_CONNECT_TYPE", CONNECT_TYPE_DECISIVES);
	TQuery q(str(m));

	if (q.Read())
	{
		connect << q;

		if (q.Read())
		{
			error("%s %ld/%ldг. има 2+ определящи вида дела на други институции/съдебни инстанции!",
				Type->Seek(key.type), key.no, key.year);
			// 2007:025 LPR: return the 1st type anyway
			//connect.connectType = '\0';
		}
	}
	else
		connect.connectType = '\0';

	return connect.connectType;
}

// ----- TDatedContainer -----------------------------------------------------
TDatedContainer::TDatedContainer(const char *name, TGroup *(*const tClone)())
	: TRCDKeyContainer(name, tClone)
{
	kind = *kind2ses_kinds(Default->kind);

	if (!kind)
		kind = *KIND_VISIBLE_SESSIONS;
}

TDatedContainer::TDatedContainer(TRCDKeyContainer *container, TGroup *(*const tClone)())
	: TRCDKeyContainer(container->Name, tClone)
{
	key = container->key;
	kind = container->kind;
	date = container->date;
}

bool TDatedContainer::Combine(TRCDKeyContainer *source)
{
	// 2003:283 Based on Insert(TDatedContainer)
	if (!TGroup::Insert())
		return false;

	Signal(ELECTRIC_ADD);

	if (source)
	{
		mstr involvements;
		TSideWind sideWind;
		TLinkWind linkWind;

		Involvements(involvements);
		sideWind.Transfer(source, this, str(involvements));
		linkWind.Transfer(source, this, str(involvements));
	}

	return true;
}

void TDatedContainer::ExactCriteria(msql &m, const char *fields)
{
	TRCDKeyContainer::ExactCriteria(m, fields);
	m.Add(this, "F_KIND, F_DATE");
}

void TDatedContainer::FloatCriteria(msql &m, const char *fields)
{
	TRCDKeyContainer::ExactCriteria(m, fields);
}

bool TDatedContainer::Insert()
{
	TDummyContainer source;

	source << this;
	// 2004:007 BUGFIX: if a fromDate exists, it's a better maximum, no
	// 2004:007 BUGFIX: matter if sideWinds are at date or at fromDate
	TDate *fromDate = (TDate *) Find("F_FROM_DATE");

	if (fromDate && !fromDate->Date().Empty())
		source.date = fromDate->Date();

	// Generic: search for same document (not event!) as itself
	return Combine(Latest(&source) ? &source : NULL);
}

void TDatedContainer::Involvements(mstr &m) const
{
	TRCDKeyContainer::Involvements(m);

	if (strchr(TYPE_LAWSUITS, key.type))
	{
		TLawsuit lawsuit;
		lawsuit.key = key;

		if (lawsuit.Get("F_KIND, F_SOURCE_KIND"))		// 2010:154 LPR: F_KIND, F_SOURCE_KIND
			lawsuit.Involvements(m);
	}
	// 2005:319 LPR: at least for request resolutions
	else if (key.type == TYPE_REQUEST)
	{
		TRequest request;
		request.key = key;

		if (request.Get("F_KIND"))				// 2010:154 LPR: F_KIND
			request.Involvements(m);
	}
	// 2007:282 LPR: at least for surrounds; also for inreg events
	else if (key.type == TYPE_INREG)
	{
		TInReg inReg;
		inReg.key = key;

		if (inReg.Get())
			inReg.Involvements(m);
	}
}

bool TDatedContainer::Suitor(char involvement)
{
	// 2010:302 LPR: key.type -> origin
	TRCDKey origin;

	if (find_origin(this, origin))
	{
		// 2007:170 LPR: type check
		if (strchr(TYPE_LAWSUITS, origin.type))
		{
			TLawsuit lawsuit;
			lawsuit.key = origin;

			if (lawsuit.Get("F_KIND, F_SOURCE_KIND") && lawsuit.Suitor(involvement))
				return true;
		}
		// 2009:136 LPR: handle indocs, assignments use their suitor
		else if (origin.type == TYPE_REQUEST)
		{
			TRequest request;
			request.key = origin;

			if (request.Get("F_KIND") && request.Suitor(involvement))
				return true;
		}
	}

	return TRCDKeyContainer::Suitor(involvement);
}

// ----- TIndexContainer -----------------------------------------------------
TIndexContainer::TIndexContainer(const char *name, TGroup *(*const tClone)())
	: TDatedContainer(name, tClone)
{
	Add(new TLong("F_EVENT_NO", &eventNo));
}

bool TIndexContainer::Delete(bool exact)
{
	bool deleted = TDatedContainer::Delete(exact);

	if (exact && deleted)
		DeleteIndex();

	return deleted;
}

bool TIndexContainer::DeleteIndex()
{
	if (eventNo && (DecisionIndex->type = IndexType()) != TYPE_NULL)
	{
		DecisionIndex->kind = IndexKind();
		DecisionIndex->year = IndexYear();
		DecisionIndex->eventNo = eventNo;
		return DecisionIndex->Delete(true);
	}

	return true;
}

bool TIndexContainer::IndexEvent()
{
	if ((DecisionIndex->type = IndexType()) == TYPE_NULL)
	{
	#if RANGECHECK
		if (eventNo)
			fatal("TIndexContainer::IndexEvent: eventNo");
	#endif  // RANGECHECK
		return true;
	}

	DecisionIndex->kind = IndexKind();
	// 2010:284 LPR: separated in IndexYear()
	DecisionIndex->year = IndexYear();
	DecisionIndex->eventNo = eventNo;

	if (DecisionIndex->Insert())
	{
		eventNo = DecisionIndex->eventNo;
		return true;
	}

	return false;
}

bool TIndexContainer::IndexRead()
{
	msql m(Name);

	m.Add(this, "F_NO, F_YEAR, F_TYPE, F_KIND, F_EVENT_NO");
	TQuery q(str(m));

	if (q.Read())
	{
		*this << q;		// 2005:056 BUGFIX: was missing
		return true;
	}

	return false;
}

// 2005:059 LPR: the standard Try() takes priority over eventNo
sign TIndexContainer::IndexRead(bool mandatory)
{
	// 2005:077 LRQ: check date/eventNo - moved here from GetDecDialog
	CDate tDate = date;
	long tEventNo = eventNo;

	if (!date.Empty() && Try())
	{
		if (tEventNo && tEventNo != eventNo)
		{
			// LRQ: simple single message only
			error("Въведеният номер на съдебен акт не е верен.");
			return -1;
		}

		return true;
	}

	// 2005:059 Generic autotry all-of-a-kind instead of 2 decisions only
	if (eventNo)
	{
		const char *kinds;

		if (strchr(KIND_CLOSED_SESSIONS, kind))
			kinds = KIND_CLOSED_SESSIONS;
		else if (strchr(KIND_DECISIONS, kind))
			kinds = KIND_DECISIONS;
		else
			kinds = C2S[kind];

		char tKind = kind;

		while (*kinds++)
		{
			kind = *kinds;
			if (IndexRead())
			{
				if (!tDate.Empty() && tDate != date)
				{
					// LRQ: simple single message only
					error("Въведената дата на заседание не е вярна.");
					return -1;
				}

				return true;
			}
		}

		kind = tKind;
	}

	if (mandatory)
		say_no_data(Name);

	return false;
}

TDate *TIndexContainer::IndexDate() const
{
	static constant DateNames[] = { "F_FROM_DATE", "F_DECISION_DATE", "F_DATE", NULL };
	TDate *indexDate;

	for (constant *dateName = DateNames; *dateName; dateName++)
		if ((indexDate = (TDate *) Find(*dateName)) != NULL)
			break;

	return indexDate;
}

long TIndexContainer::IndexYear() const
{
	const TDate *fromDate = (const TDate *) Find("F_FROM_DATE");
	const TDate *decisionDate = (const TDate *) Find("F_DECISION_DATE");

	return fromDate ? fromDate->Date().year : decisionDate ? decisionDate->Date().year : date.year;
}

bool TIndexContainer::Insert()
{
	if (IndexEvent())
	{
		if (TDatedContainer::Insert())
			return true;

		if (DecisionIndex->type)
			DecisionIndex->Delete(true);
	}

	return false;
}

void TIndexContainer::LoadOutRegs(TGroupArray &array) const
{
	msql m("T_OUTREG O JOIN T_INREG I ON I.F_NO = O.F_IN_NO AND I.F_YEAR = O.F_IN_YEAR",
		"O.*, I.F_DECISION_KIND AS F_DEC_KIND");
	bool session = strchr(KIND_SESSIONS, kind);
	TDecision decision;
	TOutReg outReg;

	decision.key = key;
	outReg.Add(new TChar("F_DEC_KIND", &decision.kind));
	decision.date = date;

	m.AddLong("O.F_LAWSUIT_NO", key.no);
	m.AddLong("O.F_LAWSUIT_YEAR", key.year);
	m.AddChar("O.F_LAWSUIT_TYPE", key.type);
	m.AddChar(session ? "I.F_SESSION_KIND" : "I.F_DECISION_KIND", kind);
	m.AddDate("I.F_DECISION_DATE", date);

	TQuery q(str(m));
	while (q.Read())
	{
		outReg << q;

		if (session && decision.Try("F_SESSION_KIND") && decision.sessionKind == kind)
			;  // exact act exists, same logic as AppealedDecision
		else
			array.AddCopy(&outReg);
	}
}

void TIndexContainer::OutRegCriteria(msql &w, const char *fields) const
{
	// 2014:149 LPR: start with T_OUTREG
	w.Begin("T_OUTREG O JOIN T_INREG I ON I.F_NO = O.F_IN_NO AND I.F_YEAR = O.F_IN_YEAR", fields);
	//w.Begin("T_INREG I JOIN T_OUTREG O ON I.F_NO = O.F_IN_NO AND I.F_YEAR = O.F_IN_YEAR", fields);
	// 2014:149 LPR: I.F_LAWSUIT -> O.
	w.AddLong("O.F_LAWSUIT_NO", key.no);
	w.AddLong("O.F_LAWSUIT_YEAR", key.year);
	w.AddChar("O.F_LAWSUIT_TYPE", key.type);
	// 2018-11-14: FIX: session kind support
	w.AddChar(strchr(KIND_SESSIONS, kind) ? "I.F_SESSION_KIND" : "I.F_DECISION_KIND", kind);
	w.AddDate("I.F_DECISION_DATE", date);
}

bool TIndexContainer::operator<=(const TIndexContainer &that) const
{
	return eventNo != that.eventNo ? eventNo < that.eventNo : date.year != that.date.year ? date.year <= that.date.year :
		kind != that.kind ? kind < that.kind : key.type <= that.key.type;
}

// ----- TPrimeContainer -----------------------------------------------------
TPrimeContainer::TPrimeContainer(const char *name, TGroup *(*const tClone)())
	: TDatedContainer(name, tClone)
{
	Add(
		NEW_RCDKEY("F_LAWSUIT_", &lawsuit),
		NULL
	);
}

void TPrimeContainer::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_LAWSUIT_NO, F_LAWSUIT_YEAR, F_LAWSUIT_TYPE");
}

bool TPrimeContainer::Latest(TRCDKeyContainer *container)
{
	// 2015:049 LPR: cut, we don't need any origin data
	container->key = lawsuit;
	container->date = date;
	return TDatedContainer::Latest(container);
}
