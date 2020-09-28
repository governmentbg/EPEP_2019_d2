#include "cac.h"

enum
{
	DEC_TEXT_NONE,
	DEC_TEXT_FILTERED,
	DEC_TEXT_FULL
};

class TDecisionRegisterGroup : public TBookGroup
{
public:
	TDecisionRegisterGroup();

	virtual void FormCriteria(msql &m);

	long decText;
};

TDecisionRegisterGroup::TDecisionRegisterGroup()
	: TBookGroup("T_DECISION", "F_TYPES", "F_RETURNED", false, false)
{
	Add(
		new TRadio("F_DEC_TEXT_NONE", &decText, DEC_TEXT_NONE),
		new TRadio("F_DEC_TEXT_FILTERED", &decText, DEC_TEXT_FILTERED),
		new TRadio("F_DEC_TEXT_FULL", &decText, DEC_TEXT_FULL),
		NULL
	);

	*types = '\0';
	decText = DEC_TEXT_FILTERED;
}

void TDecisionRegisterGroup::FormCriteria(msql &m)
{
	// 2016:221 FIX: read F_TEXT only if needed
	mstr fields("F_NO, F_YEAR, F_TYPE, F_KIND, F_FROM_DATE, F_RETURNED, F_EVENT_NO");

	if (decText != DEC_TEXT_NONE)
		fields.cat(", F_TEXT");

	m.Begin(Name, str(fields));
	m.AddChars("F_TYPE", types, TYPE_LAWSUITS);
	m.AddRange("F_RETURNED", minDate, maxDate);
	m.AddChars("F_KIND", KIND_DECISIONS, KIND_ENDOCS);
	// 2016:182
	//m.AddOrder("F_RETURNED, F_EVENT_NO, F_TYPE, F_YEAR, F_NO"); -> TGSArray
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
	if (intersect(types, TYPE_TRADEALIKE_LAWS))
	{
		fields.cpy("F_LAWSUIT_NO AS F_NO, F_LAWSUIT_YEAR AS F_YEAR, F_LAWSUIT_TYPE AS F_TYPE, "
			"F_DECISION_KIND AS F_KIND, F_DECISION_DATE AS F_FROM_DATE, F_DECISION_DATE AS F_RETURNED, F_EVENT_NO");

		if (decText != DEC_TEXT_NONE)
			fields.cat(", F_TEXT");

		msql w("T_SURROUNDMENT", str(fields));
		w.AddChars("F_LAWSUIT_TYPE", types, TYPE_LAWSUITS);
		w.AddRange("F_DECISION_DATE", minDate, maxDate);
		w.AddChars("F_DECISION_KIND", KIND_DECISIONS, KIND_ENDOCS);
		m.cat(" UNION ");
		m.cat(w);
	}
#endif  // APPEAL || AREAL
}

class TRegisterDecision : public TDecision
{
public:
	TRegisterDecision(TQuery &q, long tFilterText);
	bool operator<=(const TRegisterDecision &that) const;

	long filterText;
};

TRegisterDecision::TRegisterDecision(TQuery &q, long tFilterText)
	: filterText(tFilterText)
{
	*this << q;
}

bool TRegisterDecision::operator<=(const TRegisterDecision &that) const
{
	return returned != that.returned ? returned < that.returned : eventNo != that.eventNo ? eventNo < that.eventNo :
		key <= that.key;
}

#define decision OFFICE_GROUP_CAST(TRegisterDecision)

static void decReg_lawsuit_kind(mstr &m, TGroup *group)
{
	TLawsuit lawsuit;
	lawsuit.key = decision->key;

	if (lawsuit.Get("F_KIND"))
	{
		Kind->Seek(lawsuit.kind);
		m.cpy(decision->filterText == DEC_TEXT_NONE ? Kind->name : Kind->alias);
	}
}

static void decReg_event_kind(mstr &m, TGroup *group) { m.cpy(Kind->Seek(decision->kind)); }
static void decReg_event_date(mstr &m, TGroup *group) { decision->fromDate.Print(m); }
static void decReg_returned(mstr &m, TGroup *group) { decision->returned.Print(m); }

static void decReg_event_text(mstr &m, TGroup *group)
{
	if (decision->filterText == DEC_TEXT_FILTERED)
		filter_personal(decision, decision->text, 1);

	m.cpy(decision->text);
}

#undef decision

static const offield decReg_fields[]=
{
	{ "F_LAWSUIT_KIND", decReg_lawsuit_kind },
	{ "F_LAWSUIT_NO", keygroup_flag_no },
	{ "F_LAWSUIT_YEAR", keygroup_key_year },
	{ "F_EVENT_KIND", decReg_event_kind },
	{ "F_EVENT_NO", container_event_no },
	{ "F_EVENT_DATE", decReg_event_date },
	{ "F_RETURNED", decReg_returned },
	{ "F_EVENT_TEXT", decReg_event_text },
	{ NULL, NULL }
};

#define decReg OFFICE_GROUP_CAST(TDecisionRegisterGroup)

//static void decReg_key_type(mstr &m, TGroup *group) { m.cpy(Type->Seek(decReg->type)); }
static void decReg_min_date(mstr &m, TGroup *group) { decReg->minDate.Print(m); }
static void	pls_shash(mstr &m, TGroup *group) { m.cpy(decReg->minDate.Empty() ? "до" : "-"); }

static void decReg_max_date(mstr &m, TGroup *group)
{
	(decReg->maxDate.Empty() ? Today : decReg->maxDate).Print(m);
}

#undef decReg

static const offield decReg_title_fields[]=
{
	{ "F_LAWSUIT_TYPE", book_types },
	{ "F_MIN_DATE", decReg_min_date },
	{ "F_SLASH", pls_shash },
	{ "F_MAX_DATE", decReg_max_date },
	{ NULL, NULL }
};

class TDecisionRegisterBook : public TBook
{
public:
	TDecisionRegisterBook(TWindow *parent, TBookGroup *group, long tFilterText);

protected:
	long filterText;

	virtual TGroup *Process(TQuery &q);
};

TDecisionRegisterBook::TDecisionRegisterBook(TWindow *parent, TBookGroup *group, long tFilterText)
	: TBook(parent, group, tFilterText == DEC_TEXT_NONE ? "DecReg1.htm" : "DecReg.htm", decReg_fields,
		decReg_title_fields, BOOK_FIXED_DATES, new TGSArray<TRegisterDecision>), filterText(tFilterText)
{
}

TGroup *TDecisionRegisterBook::Process(TQuery &q)
{
	return new TRegisterDecision(q, filterText);
}

#include "DecisionRegisterDialog.h"
#include "DecisionRegisterDialog.cpp"

void decision_register(TWindow *parent)
{
	TDecisionRegisterGroup group;

	if (TDecisionRegisterDialog(parent, &group).Execute() == IDOK)
		TDecisionRegisterBook(parent, &group, group.decText).Execute(BOOK_EXEC_HEADER);
}
