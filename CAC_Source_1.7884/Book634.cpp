#include "cac.h"

#if COURT_TYPE == COURT_AREAL
enum
{
	SESSION_634 	= 1,	// only session
	SURROUND_634 	= 2,	// only surround
	SUB_SURROUND_634 	= 3,	// secondary lines
	INREG_634 		= 4,	// only inreg
	SUB_INREG_634 	= 5,	// secondary lines
	DECISIONS_634	= 8,	// only Decisions -> session
	INREG_EVENTS_634	= 9	// only InRegEvent -> InReg
};

// --------- TBook634Group ----------
class TBook634Group : public TBookGroup
{
public:
	TBook634Group(const CDate &tMinDate, const CDate &tMaxDate, char type, long no, long year, long startNo);
	virtual void FormCriteria(msql &m);

	CDate tminDate, tmaxDate;
	long no;
	long beginNo;
	char sourceKind;
	char kind;
};

TBook634Group::TBook634Group(const CDate &tMinDate, const CDate &tMaxDate, char type, long noLaw, long yearLaw, long startNo)
	:  TBookGroup("T_LAWSUIT", "F_TYPE", "F_DATE", true, true)
{
	TLawsuit lawsuit;

	minDate = tminDate = tMinDate;
	maxDate = tmaxDate = tMaxDate;
	lawsuit.key.type = types[0] = type;
	lawsuit.key.no = no = noLaw;
	lawsuit.key.year = year = yearLaw;

	if (startNo > 0)
		beginNo = startNo;
	else beginNo = 1;

	if (lawsuit.Try())
	{
		sourceKind = lawsuit.sourceKind;
		kind = lawsuit.kind;
	}
	else
		sourceKind = KIND_INVALID;
}

void TBook634Group::FormCriteria(msql &m)
{
	m.Begin("T_LAWSUIT", "F_NO, F_TYPE, F_YEAR, F_KIND, F_DATE, F_JUDGE, F_COMPOSITION, F_SOURCE_KIND");
	m.AddLong("F_NO", no);
	m.AddLong("F_YEAR", year);
	m.AddChar("F_TYPE", types[0]);
}

#define title634 OFFICE_GROUP_CAST(TBook634Group)

static void t634_title_kind(mstr &m, TGroup *group) { m.cat(KindAlias->Seek(title634->kind)); }
static void t634_title_no(mstr &m, TGroup *group) {itom(title634->no, m);}
static void t634_title_year(mstr &m, TGroup *group) {itom(title634->year, m);}
static void t634_period(mstr &m, TGroup *group) { m.printf("%0D - %0D", &(title634->tminDate), &(title634->tmaxDate)); }

#undef title634

static const offield book634_head[]=
{
	{ "F_LAWSUIT_KIND", t634_title_kind },
	{ "F_LAWSUIT_NO", t634_title_no },
	{ "F_LAWSUIT_YEAR", t634_title_year },
	{ "F_PERIOD", t634_period },
	{ NULL, NULL }
};

class T634InRegEvent : public TInRegEvent
{
public:
	bool operator<=(const T634InRegEvent &that) const { return date < that.date; }
};

class T634subDocs : public TDummyContainer
{
public:
	TGSArray<T634InRegEvent> inRegEvents;
};

// -------- T634SubEvent ---------
class T634SubEvent : public TDummyContainer
{
public:
	TGSArray<T634subDocs> subEvents;
	bool operator<=(const T634SubEvent &that) const;
	int order;
};

bool T634SubEvent::operator<=(const T634SubEvent &that) const
{
	CDate thatDate = that.date, oldDate = date;
	int thatNo = that.key.no, oldNo = key.no;
	int thatOrder = 0, oldOrder = 0;

	if (!strchr(KIND_SURROUNDS, that.kind))
	{
		Kind->Seek(that.kind);
		thatOrder = Kind->Order();
	}
	if (!strchr(KIND_SURROUNDS, kind))
	{
		Kind->Seek(kind);
		oldOrder = Kind->Order();
	}

	if (strchr(KIND_SESSIONS, that.kind) && that.subEvents.Count() &&
		(strchr(KIND_SURROUNDS, that.subEvents[0]->kind)))
	{
		thatDate = that.subEvents[0]->date;
		thatNo = that.subEvents[0]->key.no;
	}

	if (strchr(KIND_SESSIONS, kind) && subEvents.Count() &&
		(strchr(KIND_SURROUNDS, subEvents[0]->kind)))
	{
		oldDate = subEvents[0]->date;
		oldNo = subEvents[0]->key.no;
	}

	if (thatOrder != 0 && oldOrder != 0) // sravnqvane na zasedaniq
		return thatDate != oldDate ? oldDate < thatDate : oldOrder < thatOrder;
	else
		return thatDate != oldDate ? oldDate < thatDate : oldNo < thatNo;
}

// -------- T634Identity ---------
class T634Identity : public TTransGroup
{
public:
	mstr address;
};

// -------- T634Lawsuit ---------
class T634Lawsuit : public TLawsuit
{
public:
	TGSArray<T634SubEvent> events;
	TGArray<T634Identity> identity;

	int no;
	int subNo;
	int subEventNo;
	int subcount;
	int typeDoc;
	long beginNo;

};

// -------- T634SideGroup ---------
class T634SideGroup : public TInconBaseGroup
{
public:
	T634SideGroup(TLawsuit *tLawsuit);
	void FormCriteria(msql &m);

protected:
	TLawsuit lawsuit;
};

T634SideGroup::T634SideGroup(TLawsuit *tLawsuit)
	: TInconBaseGroup("T_SIDE_WIND S", "S.F_UCN, S.F_UCN_TYPE", -1)
{
	lawsuit << *tLawsuit;
}

void T634SideGroup::FormCriteria(msql &m)
{
	BeginCriteria(m);

	m.AddLong("S.F_NO", lawsuit.key.no);
	m.AddLong("S.F_YEAR", lawsuit.key.year);
	m.AddChar("S.F_TYPE", lawsuit.key.type);
	m.AddChar("S.F_KIND", lawsuit.kind);
	m.AddDate("S.F_DATE", lawsuit.date);

	TInconBaseGroup::FormCriteria(m);
}

class T634BookDecision : public TDecision
{
public:
	bool operator<=(const T634BookDecision &that) const { return TRCDKeyContainer::operator<=(that); }
};

#define t634Law OFFICE_GROUP_CAST(T634Lawsuit)
static void t634_style(mstr &m, TGroup *group) 
{ 
	int count = t634Law->events[t634Law->no]->subEvents.Count();
	if (count && t634Law->typeDoc != SURROUND_634 && t634Law->typeDoc != INREG_634)
	{	
		m.cpy("border:none;border-right:solid"); // BEZ GORNI I DOLNI SAMO STRANICHNI !!!!!
	// 	if (t634Law->typeDoc == SES_SUR_1 && count > 1)
	//		m.cpy("border-bottom:none;border-top:solid windowtext .5pt;border-right:solid");
		if (t634Law->subNo == 0 && t634Law->typeDoc != SUB_INREG_634 && t634Law->typeDoc != SUB_SURROUND_634)
			m.cpy("border-bottom:none; border-top: solid windowtext .5pt; border-right:solid");
		else if (t634Law->subNo == count - 1)
			m.cpy("border-top:none;border-bottom:solid windowtext .5pt;border-right:solid");
	}
}

static void t634_style2(mstr &m, TGroup *group)
{
	int count = t634Law->events[t634Law->no]->subEvents.Count();
	if (count)
	{	
		int countSubEvent = t634Law->events[t634Law->no]->subEvents[t634Law->subNo]->inRegEvents.Count();
		if (countSubEvent)
		{
			int brrr = countSubEvent ? countSubEvent - 1 : 0;
			m.cpy("border:none;border-right:solid");
			if (t634Law->subNo == 0 && t634Law->subEventNo == 0 && t634Law->typeDoc != SUB_INREG_634) // otgore
				m.cpy("border-bottom:none;border-top: solid windowtext .5pt;border-bottom:none;border-right:solid");
			else if ((t634Law->subNo == count - 1) && (t634Law->subEventNo == brrr))
				m.cpy("border-top:none; border-bottom:solid windowtext .5pt;border-right:solids");
		}
		else if (count != 1)
			m.cpy("border-bottom:none; border-top:solid windowtext .5pt;border-right:solids");
	}
}

static void t634_subcount(mstr &m, TGroup *group) { subcount(m, t634Law->subcount); }
static void t634_law_no(mstr &m, TGroup *group){ itom(t634Law->key.no, m); }
static void t634_law_year(mstr &m, TGroup *group){ itom(t634Law->key.year, m); }

static void t634_judge(mstr &m, TGroup *group)
{
	TCitizen citizen;

	if (t634Law->typeDoc == SESSION_634)
	{
		TSession session;
		session << t634Law->events[t634Law->no];

		if (session.Get("F_JUDGE"))
			strcpy(citizen.ucn, session.judge);
	}
	else if (t634Law->typeDoc == INREG_EVENTS_634)  // 18.06.2012
	{
		TInRegEvent inRegEvent;
		inRegEvent << t634Law->events[t634Law->no];

		if (inRegEvent.Get("F_JUDGE") && atob(inRegEvent.judge))
			strcpy(citizen.ucn, inRegEvent.judge);
		else
			strcpy(citizen.ucn, t634Law->judge);
	}
	// 2014:155 TRQ: print surround judge
	else if (t634Law->typeDoc == SURROUND_634)
	{
		TSurround surround;
		surround << t634Law->events[t634Law->no];

		if (surround.Get("F_JUDGE") && atob(surround.judge))
			strcpy(citizen.ucn, surround.judge);
		else
			strcpy(citizen.ucn, t634Law->judge);
	}
	else
		strcpy(citizen.ucn, t634Law->judge);

	print_user(m, citizen.ucn, UCN_CITIZEN_UCN, true, -1);
}

static void t634_compsition(mstr &m, TGroup *group) { print_composition(m, t634Law->composition, "с-в"); }

static void T634_deptor(mstr &m, TGroup *group)
{
	for (int i = 0; i < t634Law->identity.Count(); i++)
	{
		m.cat(t634Law->identity[i]->name);
		if (any(t634Law->identity[i]->reName))
		{
			m.sep(" ");
			m.cat(t634Law->identity[i]->reName);
		}
		if (any(t634Law->identity[i]->family))
		{
			m.sep(" ");
			m.cat(t634Law->identity[i]->family);
		}
		if (any(t634Law->identity[i]->reFamily));
		{
			m.sep(" ");
			m.cat(t634Law->identity[i]->reFamily);
		}
		m.sep("\v");
		m.cat(t634Law->identity[i]->address);
	}
}

static void t634_no(mstr &m, TGroup *group){ itom(t634Law->beginNo, m); }

static void t634_document_date(mstr &m, TGroup *group)
{
	int typeDoc = t634Law->typeDoc;
	if (typeDoc == SURROUND_634 || typeDoc == INREG_634) // dokument koito e sam na red
		m.printf("%0D", &t634Law->events[t634Law->no]->date);
	else if (typeDoc != SESSION_634 && typeDoc != DECISIONS_634 && typeDoc != INREG_EVENTS_634)
		m.printf("%0D", &t634Law->events[t634Law->no]->subEvents[t634Law->subNo]->date);
//	else
//		m.printf("%0D", &t634Law->events[t634Law->no]->date);
}

static void t634_document(mstr &m, TGroup *group)
{
	int typeDoc = t634Law->typeDoc;
	if (typeDoc != INREG_634 && typeDoc != SUB_INREG_634 && typeDoc != SESSION_634
		&& typeDoc != DECISIONS_634 && typeDoc != INREG_EVENTS_634)
	{
		TSurround surround;

		if (t634Law->events[t634Law->no]->subEvents.Count() > 0)
			surround << t634Law->events[t634Law->no]->subEvents[t634Law->subNo];
		else
			surround << t634Law->events[t634Law->no];

		if (surround.Try())
		{
			// 2016:293 TRQ: surround action -> name, not text
			//if (surround.kind == KIND_ACTION_SURROUND || strchr(KIND_ANY_OTHER_DOCUMENTS, surround.kind))
			if (surround.kind != KIND_ACTION_SURROUND && strchr(KIND_ANY_OTHER_DOCUMENTS, surround.kind))
			{
				m.cpy(surround.text);
				m.sep(" ");
				m.printf("Вх. № %d/%d", surround.key.no, surround.key.year);
			}
			else
			{
				m.cat(Kind->Seek(surround.kind));
				m.sep("\v");
				mstr n;

				n.cpy(surround.text);
				n.sep(" ");
				n.printf("Вх. № %d/%d", surround.key.no, surround.key.year);
				m.cat(n);
			}
		}
	}
	else if ((typeDoc == INREG_634 || typeDoc == SUB_INREG_634) && typeDoc != SESSION_634)
	{
		TInReg inReg;

		if (t634Law->events[t634Law->no]->subEvents.Count() > 0)
			inReg << t634Law->events[t634Law->no]->subEvents[t634Law->subNo];
		else
			inReg << t634Law->events[t634Law->no];

		if (inReg.Try())
		{
			if (strchr(KIND_ANY_OTHER_DOCUMENTS, inReg.kind))
			{
				m.cat(inReg.text);
				m.sep(" ");
				m.printf("Вх. № %d/%d", inReg.key.no, inReg.key.year);
			}
			else
			{
				m.cat(Kind->Seek(inReg.kind));
				m.sep("\v");
				mstr n;

				n.cpy(inReg.text);
				n.sep(" ");
				n.printf("Вх. № %d/%d", inReg.key.no, inReg.key.year);
				m.cat(n);
			}
			if (typeDoc == INREG_634)
			{
				if (inReg.decisionKind && inReg.sessionKind)
				{
					TKind tKind;
					m.printf(" срещу  %s от %s ", tKind.Seek(inReg.decisionKind),
							Kind->Seek(inReg.sessionKind));

					if (!inReg.decisionDate.Empty())
						m.printf("от %D", &inReg.decisionDate);
				}
			}
		}
	}
}

static void t634_senders(mstr &m, TPrimeContainer &container)
{
	if (container.Try())
	{
		TGArray<TSideWind> sideWinds;
		load_sidebasicv(&container, sideWinds);
		for (int i = 0; i < sideWinds.Count(); i++)
		{
			if (sideWinds[i]->Appealer())
			{
				TSideWind sideWind;
				sideWind << sideWinds[i];
				sideWind << container;
				mstr p;
				print_links_names(p, &sideWind);
				m.sep("\v");
				m.cat(p);
			}
		}
	}
}

static void t634_sender(mstr &m, TGroup *group)
{
	int typeDoc = t634Law->typeDoc;
	if (typeDoc != INREG_634 && typeDoc != SUB_INREG_634 && typeDoc != SESSION_634 &&
		typeDoc != DECISIONS_634 && typeDoc != INREG_EVENTS_634) // 19.06.2012
	{
		TSurround surround;

		if (t634Law->events[t634Law->no]->subEvents.Count() > 0)
			surround << t634Law->events[t634Law->no]->subEvents[t634Law->subNo];
		else
			surround << t634Law->events[t634Law->no];

		t634_senders(m, surround);
	}
	else if (typeDoc == INREG_634 || typeDoc == SUB_INREG_634 ||
			typeDoc == INREG_EVENTS_634) // 19.06.2012
	{
		TInReg inReg;

		if (t634Law->events[t634Law->no]->subEvents.Count() > 0)
			inReg << t634Law->events[t634Law->no]->subEvents[t634Law->subNo];
		else
			inReg << t634Law->events[t634Law->no];

		 t634_senders(m, inReg);
	}
}

static void t634_printDecisionDate(mstr &m, T634BookDecision *decision, bool printResult = true)
{
	m.sep("\v");
	m.cat(Kind->Seek(decision->kind));  // TYPE -> KIND

	if (PRINT_JUDGEMENTS && decision->eventNo)
		m.printf("\v№ %d", decision->eventNo);
	m.printf(" от %0D", &decision->fromDate);
	if (!strchr(RESULT_WITH_READY_ANYS, decision->result) && printResult)
	{
		m.sep("\v");
		m.cat(Result->Seek(decision->result));
	}
	if (printResult)
	{
		m.sep("\v");
		m.cat(decision->text);
	}
}

static void t634_print_InregEvent(mstr &m, T634InRegEvent &inRegEvent)
{
	m.cat(Result->Seek(inRegEvent.result));

	if (!strchr(RESULT_IOREG_RETURNEDS, inRegEvent.result) && PRINT_JUDGEMENTS) /// --- 28.09.2012
		m.printf(" № %d", inRegEvent.outNo ? inRegEvent.outNo : inRegEvent.eventNo);

	m.printf(" %0D", &inRegEvent.date);

	if (any(inRegEvent.text))
	{
		m.sep("\v");
		m.cat(inRegEvent.text);
	}
}

static void t634_event(mstr &m, TGroup *group)
{
	int typeDoc = t634Law->typeDoc; 

	if ((typeDoc == INREG_634 || typeDoc == SUB_INREG_634) && typeDoc != SUB_SURROUND_634 &&
		(t634Law->events[t634Law->no]->subEvents[t634Law->subNo]->inRegEvents.Count()))
	{
		T634InRegEvent inRegEvent;
		inRegEvent << t634Law->events[t634Law->no]->subEvents[t634Law->subNo]->inRegEvents[t634Law->subEventNo];
		t634_print_InregEvent(m, inRegEvent);
	}
	else if ((typeDoc == DECISIONS_634 || typeDoc == INREG_EVENTS_634) && typeDoc != SUB_SURROUND_634)
	{
		T634InRegEvent inRegEvent;
		T634BookDecision decision;
		if (typeDoc == DECISIONS_634)
		{
			decision << t634Law->events[t634Law->no];
			decision.Get();
		}
		else
		{
			TInReg inReg;
			inRegEvent << t634Law->events[t634Law->no];
			inReg << t634Law->events[t634Law->no];
			if (inReg.Get())
			{
				decision.key = inReg.lawsuit;
				decision.kind = inReg.decisionKind;
				decision.date = inReg.decisionDate;
			}
			if (!decision.Try())
			{
				if (decision.kind != KIND_SETTLEMENT)
					say_no_data(decision.Name);
				return;
			}
		}

		m.printf("%s от %0D.", KindAlias->Seek(decision.sessionKind), &decision.date);

		if (typeDoc == DECISIONS_634)
		{
			t634_printDecisionDate(m, &decision);
			if (t634Law->events[t634Law->no]->subEvents.Count() > 0) m.sep("\v\v");
		}
		else
		{
			t634_printDecisionDate(m, &decision, false);
			if (inRegEvent.Try())
			{
				m.sep("\v");
				t634_print_InregEvent(m, inRegEvent);
			}
		}
	}
	else if (typeDoc != SURROUND_634 && typeDoc != SUB_SURROUND_634)
	{
		TSession session;
		TGSArray<T634BookDecision> decisions;

		session << t634Law->events[t634Law->no];

		if (session.Try())
		{
			m.printf("%s от %0D.", KindAlias->Seek(session.kind), &session.date);
			m.sep("\v");
			m.cat(Result->Seek(session.result));
			m.sep("\v");
			m.cat(session.text);

			if (try2decide(&session, &decisions, NULL, NULL))
			{
				for (int i = 0; i < decisions.Count(); i++)
					t634_printDecisionDate(m, decisions[i]);
			}

			if (t634Law->events[t634Law->no]->subEvents.Count() > 0)
				m.sep("\v\v");
		}
	}
	else if (typeDoc == SUB_SURROUND_634) // -------------- 17.07.2012 -----
	{
		TSurround surround;
		TSurroundment surroundment;
		msql w;

		surround << t634Law->events[t634Law->no]->subEvents[t634Law->subNo];
		surround.Get();
		surroundment << surround;
		surroundment.FloatCriteria(w);
		TQuery q(str(w));

		while (q.Read())
		{
			surroundment << q;
			if (!strchr(RESULT_WITH_READY_ANYS, surroundment.result))
				m.cat(Result->Seek(surroundment.result));

			if (surroundment.decisionKind)
			{
				m.printf("%s ", Kind->Seek(surroundment.decisionKind));

				if (surroundment.eventNo && PRINT_JUDGEMENTS)
					m.printf("№ %ld/", surroundment.eventNo);
				else
					m.puts("от ");

				m.printf("%0D.", &surroundment.decisionDate, Kind->Seek(surroundment.kind));
			}

			if (any(surroundment.text))
			{
				m.sep("\v");
				m.cat(surroundment.text);
			}
			m.sep("\v\v");
		}
	}
}

static void t634_instance(mstr &m, TGroup *group, char instance)
{
	int typeDoc = t634Law->typeDoc;
	if ((typeDoc == INREG_634 || typeDoc == SUB_INREG_634) &&
		t634Law->events[t634Law->no]->subEvents[t634Law->subNo]->inRegEvents.Count())
	{
		T634InRegEvent inRegEvent;
		TOutReg outReg;
		inRegEvent << t634Law->events[t634Law->no]->subEvents[t634Law->subNo]->inRegEvents[t634Law->subEventNo]; // напълно зареден InReg

		outReg.key.type = TYPE_OUTREG;
		outReg.key.no = inRegEvent.outNo;
		outReg.key.year = inRegEvent.outYear;

		if (outReg.Try() && outReg.receiverType == UCN_INSTANCE_HIGHER && strchr(RESULT_IOREG_RETURNEDS, inRegEvent.result))
		{
			if (instance == CONNECT_TYPE_VZ_LAWSUIT)
				m.cat(Resolution->Seek(outReg.resultI));
			else
				m.cat(Resolution->Seek(outReg.resultKS));

			TConnect connect;

			connect.key = outReg.key;
			connect.connectType = instance;

			if (connect.Try())
			{
				m.sep("\v");
				m.printf("%s № %s/%ld", ConnectKind->Seek(connect.connectKind), connect.connectNo, connect.connectYear);
				TSender sender;
				sender << connect;
				if (sender.Try()) m.printf(" на %s", sender.name);
			}

			if (instance == CONNECT_TYPE_VZ_LAWSUIT)
			{
				if (any(outReg.remarkVZ))
				{
					m.cat("\v");
					m.cat(outReg.remarkVZ);
				}
				m.sep("\v");
				m.cat(Resolution->Seek(outReg.resultII));
			}
			else
			{
				if (any(outReg.remarkKS))
				{
					m.cat("\v");
					m.cat(outReg.remarkKS);
				}
			}
		}
	}
}

static void t634_appeal_result_i(mstr &m, TGroup *group) { t634_instance(m, group, CONNECT_TYPE_VZ_LAWSUIT); }
static void t634_appeal_result_ks(mstr &m, TGroup *group) { t634_instance(m, group, CONNECT_TYPE_KS_LAWSUIT); }

#undef t634Law

static const offield book634_body[]=
{
	{ "F_STYLE", t634_style },
	{ "F_STYLE2", t634_style2 },
	{ "F_SUBCOUNT", t634_subcount },
	{ "F_LAWSUIT_NO", t634_law_no },
	{ "F_LAWSUIT_YEAR", t634_law_year },
	{ "F_JUDGE", t634_judge },
	{ "F_COMPOSITION", t634_compsition },
	{ "F_DEBTOR", T634_deptor },
	{ "F_NO", t634_no },
	{ "F_DOCUMENT_DATE", t634_document_date },
	{ "F_DOCUMENT", t634_document },
	{ "F_SENDER", t634_sender },
	{ "F_EVENT", t634_event },
	{ "F_APPEAL_RESULT_I", t634_appeal_result_i },
	{ "F_APPEAL_RESULT_KS", t634_appeal_result_ks },
	{ NULL, NULL }
};

// -------- T634Book ---------
// 2015:271 LPR: general optimizations and cleanup
class T634Book : public TBook
{
public:
	T634Book(TWindow *parent, TBook634Group &tGroup, const char * fileName, const offield * bodyFields, const offield *bookFields);

	T634Lawsuit t634Array;
	TGSArray<TRCDKeyContainer> events;

protected:
	virtual TGroup *Process(TQuery &q);
	virtual void Markers();
	virtual void EmitRow(TGroup *group);

private:
	void ContainerEvents(TDatedContainer *container, TGSArray<TRCDKeyContainer> *array, const char *date = NULL);
	void QueryEvents(TLawsuit *lawsuit, TGSArray<TRCDKeyContainer> *array);
	bool HasSurround(const TRCDKeyContainer *keySession, const TRCDKeyContainer *keyContainer);
	bool HasInReg(const TRCDKeyContainer *keySession, const TRCDKeyContainer *keyContainer, bool isDecision = false);
	bool HasInRegEvent(const TRCDKeyContainer *keyInReg, const TRCDKeyContainer *keyInregEvent);
	void NomInRegEvent(T634subDocs *keyInReg);
	void PreviousSurround(TRCDKeyContainer *keySession, TGSArray<T634subDocs> &subEvents);
	void AfterInReg(TRCDKeyContainer *keySession, TGSArray<T634subDocs> &subEvents, bool isDecision = false);

	CDate minDate, maxDate;
	long count;
	long beginNo; 
	const char *tr_startFin;
	const char *sub1_start;
	const char *sub1_final;
	const char *sub2_start;
	const char *sub2_final;
	const char *sub3_start;
	const char *sub3_final;
};

T634Book::T634Book(TWindow *parent, TBook634Group &tGroup, const char *fileName, const offield *bodyFields, const offield *bookFields)
	: TBook(parent, &tGroup, fileName, bodyFields, bookFields)
{
	minDate = tGroup.minDate;
	maxDate = tGroup.maxDate;
	beginNo = tGroup.beginNo;
	count = 0;
}

TGroup *T634Book::Process(TQuery &q)
{
	msql w;
	TLawsuit *lawsuit = new TLawsuit;

	*lawsuit << q;
	t634Array << *lawsuit;

	T634SideGroup sideWindGroup(lawsuit);
	sideWindGroup.FormCriteria(w);

	TQuery p(str(w));
	while (p.Read())
	{
		T634Identity *trGroup = new T634Identity();
		*trGroup << p;

		if (strchr(UCN_CITIZENS, trGroup->ucnType))
		{
			TCitizen citizen;
			strcpy(citizen.ucn, trGroup->ucn);
			citizen.ucnType = trGroup->ucnType;
			if (citizen.Try()) { *trGroup << citizen; }
			TAddress tAddress;
			tAddress << citizen;
			if (tAddress.Try())
				tAddress.Print(trGroup->address, ADDRESS_DISTINCT);
		}
		else
		{
			TFirm firm;
			strcpy(firm.ucn, trGroup->ucn);
			firm.ucnType = trGroup->ucnType;
			if (firm.Try()) { *trGroup << firm; }
			TAddress tAddress;
			tAddress << firm;
			if (tAddress.Try())
				tAddress.Print(trGroup->address, ADDRESS_DISTINCT);
		}

		t634Array.identity.Add(trGroup);
	}

	// Dobavqt se vsichki sybitiq v t634Array.events i se syzdava prazen spisyk ot eleneti v t634Array.subEvents
	QueryEvents(lawsuit, &events);
	int eventCount;

	for (int i = 0; i < (eventCount = events.Count()); i++)
	{
		T634SubEvent *event = new T634SubEvent();
		*event << events[i];
		if (strchr(KIND_IN_REG_DOCS, event->kind))		// Съпровождащ документ
		{
			for (int j = 0; j < events.Count(); j++)
			{
				if (strchr(KIND_INREG_EVENTS, events[j]->kind) && HasInRegEvent(event, events[j]))
				{
					events.Remove(j);
					i > j ? i--, j-- : j-- ;
				}
			}
		}
		delete event;
	}

	for (int i = 0; i < (eventCount = events.Count()); i++)
	{
		int percent = i * (100.0 / (eventCount ? eventCount : 1));

		// 2015:271
		w.clear();
		w.printf("%cД %ld/%ld - %d%%...", *Type->Seek(lawsuit->key.type), lawsuit->key.no, lawsuit->key.year, percent);
		wait->TickText(str(w));
		//wait->TickCount(percent, "% ");

		T634SubEvent *event = new T634SubEvent();
		*event << events[i];
		if (strchr(KIND_SESSIONS, event->kind)) // Заседание
		{
			for (int j = 0; j < events.Count(); j++)
			{
				TDummyContainer *otherEvent = new TDummyContainer();
				*otherEvent << events[j];
				if (strchr(KIND_SURROUNDS, otherEvent->kind) && HasSurround(event, events[j]))
				{
// -------------------------- Dobavqne na dokument kato podmnojestvo na Event
					T634subDocs *docm = new T634subDocs();
					*docm << *otherEvent;
					event->subEvents.Add(docm);
					events.Remove(j);
					i > j ? i--, j-- : j--;
//					events[j]->key.no = 0;
				}
				else if (strchr(KIND_IN_REG_DOCS, otherEvent->kind) && HasInReg(event, events[j]))
				{
// -------------------------- Dobavqne na dokument kato podmnojestvo na Event
					T634subDocs *docm = new T634subDocs();
					*docm << *otherEvent;
					NomInRegEvent(docm);
					event->subEvents.Add(docm);
					events.Remove(j);
					i > j ? i--, j-- : j-- ;
//					events[j]->key.no = 0;
				}
// -------------------------- 18.06.2012 ------
				if (strchr(KIND_DECISIONS, otherEvent->kind))
				{
					TGSArray<T634BookDecision> decisions;
					TSession session;
					session << *event;
					try2decide(&session, &decisions, NULL, NULL); // -- vsichki decisions na session
					for (int k = 0; k < decisions.Count(); k++)
						if (decisions[k]->key == otherEvent->key &&
						    decisions[k]->kind == otherEvent->kind && decisions[k]->date == otherEvent->date)
						{
							events.Remove(j);
							i > j ? i--, j-- : j-- ;
						}
				}

				delete otherEvent;
			}
			PreviousSurround(event, event->subEvents);
			AfterInReg(event, event->subEvents);
			t634Array.events.Add(event);
		}
		else
			delete event;
	}

	for (int i = 0; i < events.Count(); i++)
	{
		T634SubEvent *event = new T634SubEvent();
		*event << events[i];
		if (strchr(KIND_DECISIONS, event->kind))
		{
			for (int j = 0; j < events.Count(); j++)
			{
				TDummyContainer *otherEvent = new TDummyContainer();
				*otherEvent << events[j];
				if (strchr(KIND_IN_REG_DOCS, otherEvent->kind) && HasInReg(event, otherEvent, true))
				{
					T634subDocs *docm = new T634subDocs();
					*docm << *otherEvent;
					NomInRegEvent(docm);
					event->subEvents.Add(docm);
					events.Remove(j);
					i > j ? i--, j-- : j-- ;
				}
				delete otherEvent;
			}
			AfterInReg(event, event->subEvents, true);
			t634Array.events.Add(event);
		}
		else
			delete event;
	}

// -------------------------- END --- 18.06.2012 ----

// zapisite koito ne sa kym zasedanieto se dabavqt v t634Array.
	for (int i = 0 ; i < events.Count(); i++)
	{
		if (strchr(KIND_SURROUNDS, events[i]->kind) || strchr(KIND_INREG_EVENTS, events[i]->kind))
		{
			T634SubEvent *event = new T634SubEvent();
			*event << events[i];
			t634Array.events.Add(event);
		}
		else if (strchr(KIND_IN_REG_DOCS, events[i]->kind))  // Обжалване
		{
			T634SubEvent *event = new T634SubEvent();
			*event << events[i];
			T634subDocs *docm = new T634subDocs();
			*docm << events[i];
			NomInRegEvent(docm);
			event->subEvents.Add(docm);
			t634Array.events.Add(event);
		}
	}

	events.Flush();
	return lawsuit;
}

void T634Book::Markers()
{
	tr1start = office.Find("tr1 start");
	sub1_start = office.Find("tr1 subStart");  // Surround + InReg
	sub1_final = office.Find("tr1 subFinal");
	sub2_start = office.Find("tr2 subStart");  // Surround + InReg
	sub2_final = office.Find("tr2 subFinal");
	sub3_start = office.Find("tr3 subStart");	// 1vo samo Surround a posle ....
	sub3_final = office.Find("tr3 subFinal");
	tr1final = office.Find("tr1 final");
}

void T634Book::EmitRow(TGroup *)
{
	for (int i = 0 ; i < t634Array.events.Count() ; i++)
	{
		T634SubEvent *event = t634Array.events[i];
		T634Lawsuit *total = &t634Array;
		total->beginNo = beginNo + i;
		total->no = i;
		total->subNo = total->subEventNo = 0;		

		int docCount, subInReg, brSurround, brInReg;
		brSurround = brInReg = subInReg = 0;
		docCount = 1;
		for (int j = 0; j <event->subEvents.Count(); j++)
		{
			if (strchr(KIND_SURROUNDS, event->subEvents[j]->kind))
				brSurround++;
			else
			{
				int brSubEvent = event->subEvents[j]->inRegEvents.Count();
				subInReg += brSubEvent;
				brSubEvent == 0 ? brInReg++ : brInReg += brSubEvent;
			}
		}
		if (brInReg > 0) docCount += brInReg; 
		if (brSurround > 0)  docCount += brSurround;

		if (strchr(KIND_SESSIONS, event->kind) || strchr(KIND_DECISIONS, event->kind)) // Заседание или съдебен акт с всички документи към него
		{
			total->typeDoc = SESSION_634; // just session
			if (strchr(KIND_DECISIONS, event->kind))
			{
				brSurround = 0; // при съдебният акт няма съпров. док;
				total->typeDoc = DECISIONS_634;
				docCount = brInReg + 1; // 17.07.2012 
			}
		//	if (brSurround > 1) docCount = brSurround + brInReg;
	//		if (brSurround == 1) total->typeDoc = SES_SUR_1; // session + 1 surround
	//		else if (brSurround > 1) total->typeDoc = SES_SUR_MANY; // session + many surround
			office.Emit(tr1start, sub1_start, book634_body, NULL); // <tr>
			total->subcount = docCount;
			office.Emit(sub1_start, sub1_final, book634_body, total);	//OB
			total->subcount = 1;
			office.Emit(sub1_final, sub2_start, book634_body, total);
			total->subcount = docCount;
			office.Emit(sub2_start, sub2_final, book634_body, total);	// OB
			total->subcount = 1;
			office.Emit(sub2_final, sub3_start, book634_body, total);
			total->subcount = 1;
			office.Emit(sub3_start, sub3_final, book634_body, total);
			office.Emit(sub3_final, tr1final, book634_body, NULL); // </tr>
			
			if (brSurround > 0)
			{
				total->typeDoc = SUB_SURROUND_634;
				for (int j = 0; j < event->subEvents.Count(); j++)
				{
					total->subNo = j;
					if (strchr(KIND_SURROUNDS, event->subEvents[j]->kind))
					{
						total->subcount = 1;
						office.Emit(tr1start, sub1_start, book634_body, NULL); // <tr>
						office.Emit(sub1_final, sub2_start, book634_body, total);
						office.Emit(sub2_final, sub3_start, book634_body, total);
						office.Emit(sub3_start, sub3_final, book634_body, total);
						office.Emit(sub3_final, tr1final, book634_body, NULL); // </tr>
					}
				}
			}
			if (brInReg > 0)
			{
				total->typeDoc = SUB_INREG_634;
				for (int j = 0; j < event->subEvents.Count(); j++)
				{
					total->subNo = j;
					total->subEventNo = 0;
					if (strchr(KIND_IN_REG_DOCS, event->subEvents[j]->kind))
					{
						int brInRegEvent = event->subEvents[j]->inRegEvents.Count();
						total->subcount = brInRegEvent;

						office.Emit(tr1start, sub1_start, book634_body, NULL); // <tr>
						office.Emit(sub1_final, sub2_start, book634_body, total);
						office.Emit(sub2_final, sub3_start, book634_body, total);
						total->subcount = 1;
						office.Emit(sub3_start, sub3_final, book634_body, total);
						office.Emit(sub3_final, tr1final, book634_body, NULL); // </tr>
						
						for (int n = 1; n < brInRegEvent; n++)
						{
							total->subEventNo = n;
							office.Emit(tr1start, sub1_start, book634_body, NULL); // <tr>
							office.Emit(sub3_start, sub3_final, book634_body, total);
							office.Emit(sub3_final, tr1final, book634_body, NULL); // </tr>
						}
					}
				}
			}
		}
		else if (strchr(KIND_IN_REG_DOCS, event->kind)) // Обжалване
		{
			total->typeDoc = INREG_634;
			total->subcount = subInReg;
			office.Emit(tr1start, sub1_start, book634_body, NULL); // <tr>
			office.Emit(sub1_start, sub1_final, book634_body, total);	//Ob
			office.Emit(sub1_final, sub2_start, book634_body, total);
			office.Emit(sub2_start, sub2_final, book634_body, total);	// OB
			office.Emit(sub2_final, sub3_start, book634_body, total);
			total->subcount = 1;
			office.Emit(sub3_start, sub3_final, book634_body, total);
			office.Emit(sub3_final, tr1final, book634_body, NULL); // </tr>

			if (subInReg > 1)
			{
				for (int n = 1; n < event->subEvents[0]->inRegEvents.Count(); n++)
				{
					total->subEventNo = n;
					office.Emit(tr1start, sub1_start, book634_body, NULL); // <tr>
					office.Emit(sub3_start, sub3_final, book634_body, total);
					office.Emit(sub3_final, tr1final, book634_body, NULL); // </tr>
				}
			}
		}
		else 
		{
			if (strchr(KIND_SURROUNDS, event->kind)) total->typeDoc = SURROUND_634; // Съпровождащ документ
			if (strchr(KIND_INREG_EVENTS, event->kind)) total->typeDoc = INREG_EVENTS_634;
		
			total->subcount = 1;
			office.Emit(tr1start, sub1_start, book634_body, NULL);
			office.Emit(sub1_start, sub1_final, book634_body, total);
			office.Emit(sub1_final, sub2_start, book634_body, total);
			office.Emit(sub2_start, sub2_final, book634_body, total);
			office.Emit(sub2_final, sub3_start, book634_body, total);
			office.Emit(sub3_start, sub3_final, book634_body, total);
			office.Emit(sub3_final, tr1final, book634_body, NULL);
		}
	}
}

class TSurroundMyCritery : public TSurround
{
public:
	TSurroundMyCritery() { }

	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL);
};

void TSurroundMyCritery::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_LAWSUIT_NO, F_LAWSUIT_YEAR, F_LAWSUIT_TYPE");
}

void T634Book::QueryEvents(TLawsuit *lawsuit, TGSArray<TRCDKeyContainer> *array)
{
	TInReg inReg;
	TSession session;
	TSurroundMyCritery surround;
	T634InRegEvent inRegEvent;
	T634BookDecision decision;

	session.key = lawsuit->key;
	ContainerEvents(&session, array);
	inReg.lawsuit = lawsuit->key;
	ContainerEvents(&inReg, array);
	surround.lawsuit = lawsuit->key;
	ContainerEvents(&surround, array);
	decision.key = lawsuit->key;
	ContainerEvents(&decision, array, "F_FROM_DATE");
}

void T634Book::ContainerEvents(TDatedContainer *container, TGSArray<TRCDKeyContainer> *array, const char *date)
{
	msql m;

	container->FloatCriteria(m);
	// 2013:203 LPR/IRQ: include event dates
	if (container->key.type == TYPE_INREG)
	{
		m.AddRange("(F_DATE", minDate, maxDate);
		m.NextOr();
		m.AddRange("(SELECT COUNT(F_NO) FROM T_INREG_EVENT E WHERE E.F_NO = T_INREG.F_NO AND E.F_YEAR = T_INREG.F_YEAR "
			"AND F_DATE", minDate, maxDate);
		m.cat(") > 0)");
	}
	else
		m.AddRange(date ? date : "F_DATE", minDate, maxDate);

	container->LoadArray(array, str(m));
}

bool T634Book::HasSurround(const TRCDKeyContainer *keySession, const TRCDKeyContainer *keyContainer)
{
	msql m;
	TSurroundment surroundment;

	surroundment << keyContainer;
	surroundment.sessionKind = keySession->kind;
	surroundment.sessionDate = keySession->date;
	surroundment.ExactCriteria(m, "F_NO");

	TQuery q(str(m));
	return q.Read();
}

bool T634Book::HasInReg(const TRCDKeyContainer *keySession, const TRCDKeyContainer *keyContainer, bool isDecision)
{
	msql m;
	TInReg inReg;

	inReg.key = keyContainer->key;
	inReg.ExactCriteria(m, "F_NO");

	if (isDecision)
	{
		m.AddChar("F_DECISION_KIND", keySession->kind);
		m.AddDate("F_DECISION_DATE", keySession->date);
	}
	else
	{
		m.AddChar("F_SESSION_KIND", keySession->kind);
		m.AddDate("F_DECISION_DATE", keySession->date);
	}

	TQuery q(str(m));
	return q.Read();
}

bool T634Book::HasInRegEvent(const TRCDKeyContainer *keyInReg, const TRCDKeyContainer *keyInregEvent)
{
	msql m;
	TInRegEvent inRegEvent;

	inRegEvent << keyInReg;
	inRegEvent.FloatCriteria(m, "F_NO");

	TQuery q(str(m));
	while (q.Read())
	{
		inRegEvent << q;

		if (inRegEvent.key == keyInregEvent->key && inRegEvent.kind == keyInregEvent->kind &&
			inRegEvent.date == keyInregEvent->date)
		{
			return true;
		}
	}

	return false;
}

void T634Book::NomInRegEvent(T634subDocs *keyInReg)
{
	msql m("T_INREG_EVENT");
	// keyInReg is not a table
	m.AddLong("F_NO", keyInReg->key.no);
	m.AddLong("F_YEAR", keyInReg->key.year);
	m.AddChar("F_TYPE", keyInReg->key.type);

	TQuery q(str(m));
	while (q.Read())
	{
		T634InRegEvent *inRegEvent = new T634InRegEvent;
		*inRegEvent << q;
		keyInReg->inRegEvents.Add(inRegEvent);
	}
}

void T634Book::PreviousSurround(TRCDKeyContainer *keySession, TGSArray<T634subDocs> &subEvents)
{
	msql m("T_SURROUNDMENT M JOIN T_SURROUND S ON S.F_NO = M.F_NO AND S.F_YEAR = M.F_YEAR AND S.F_TYPE = M.F_TYPE "
		"AND S.F_KIND = M.F_KIND", "S.*");

	m.AddLong("M.F_LAWSUIT_NO", keySession->key.no);
	m.AddLong("M.F_LAWSUIT_YEAR", keySession->key.year);
	m.AddChar("M.F_LAWSUIT_TYPE", keySession->key.type);
	m.AddChar("M.F_SESSION_KIND", keySession->kind);
	m.AddDate("M.F_DATE", keySession->date);
	m.Add("S.F_DATE");
	m.printf("< %ld", minDate.Value());

	TQuery q(str(m));
	while (q.Read())
	{
		T634subDocs *docmSurr = new T634subDocs();
		*docmSurr << q;
		subEvents.Add(docmSurr);
	}
}

void T634Book::AfterInReg(TRCDKeyContainer *keySession, TGSArray<T634subDocs> &subEvents, bool isDecision)
{
	msql m("T_INREG");

	m.AddLong("F_LAWSUIT_NO", keySession->key.no);
	m.AddLong("F_LAWSUIT_YEAR", keySession->key.year);
	m.AddChar("F_LAWSUIT_TYPE", keySession->key.type);
	m.AddChar(isDecision ? "F_DECISION_KIND" : "F_SESSION_KIND", keySession->kind);
	m.AddDate("F_DECISION_DATE", keySession->date);
	m.Add("F_DATE");
	m.printf("> %ld", maxDate.Value());

	TQuery q(str(m));
	while (q.Read())
	{
		T634subDocs *docInReg = new T634subDocs();
		*docInReg << q;
		NomInRegEvent(docInReg);
		subEvents.Add(docInReg);
	}
}

void print_book634(TWindow *parent, const CDate &minDate, const CDate &maxDate, char type, long noLaw, long yearLaw,
	long startNo)
{
	TBook634Group book634Group(minDate, maxDate, type, noLaw, yearLaw, startNo);

	if (book634Group.sourceKind == KIND_INVALID)
		error("Няма такова дело.");
	else if (!strchr(KIND_INCONSISTENCE_INDOCS, book634Group.sourceKind))
		error("Делото не е по несъстоятелност.");
	else
	{
	#if SPEEDTESTING
		#pragma message speed testing only
		DWORD ticks = GetTickCount();
	#endif  // SPEEDTESTING
		T634Book(parent, book634Group, "634TLaw.htm", book634_body, book634_head).Execute(BOOK_EXEC_HEADER);
	#if SPEEDTESTING
		message("test version: book finished, %lu seconds.", (GetTickCount() - ticks) / 1000);
	#endif  // SPEEDTESTING
	}
}
#endif  // AREAL