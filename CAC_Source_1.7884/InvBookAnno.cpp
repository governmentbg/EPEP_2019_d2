#include "cac.h"

// ----- TInvBookAnnoGroup ---------------------------------------------------
class TInvBookAnnoGroup : public TBookGroup
{
public:
	TInvBookAnnoGroup();

	virtual void FormCriteria(msql &m);

	TGroupArray senders;
	long serialNoColumn;
};

TInvBookAnnoGroup::TInvBookAnnoGroup()
	: TBookGroup("T_ANNOUNCE", "F_TYPE", "F_DATE", false)
{
	Add(new TCheck("F_SERIAL_NO_COLUMN", &serialNoColumn, TRUE));
}

void TInvBookAnnoGroup::FormCriteria(msql &m)
{
	m.Begin(Name, "F_NO, F_YEAR, F_KIND, F_COURT, F_SUBPOENA_KIND, F_SUBPOENA_DATE, F_SESSION_DATE, F_SUBPOENA_TEXT, "
		"F_SIDE_NO, F_UCLP, F_STREET, F_ADR_NO, F_DELIVERER_UCN, F_SUBPOENA_RECEIVED, F_RECEIVED_STATUS, "
		"F_RECEIVED_TEXT, F_SUBPOENA_RETURNED, F_SUB_NO, F_ENTRANCE, F_FLOOR, F_APARTMENT, F_BLOCK_NO");

	m.AddArray("F_COURT", senders);
	m.AddRange("F_SUBPOENA_DATE", minDate, maxDate);
	m.AddOrder("F_SUBPOENA_DATE, F_COURT, F_YEAR, F_NO");	// == EndCriteria()
}

// ----- invAnno_subpoena_ ---------------------------------------------------
#define announce OFFICE_GROUP_CAST(TAnnounce)

static void invAnno_subpoena_date(mstr &m, TGroup *group) { announce->subpoenaDate.Print(m, "%Dг."); }
static void invAnno_subpoena_court(mstr &m, TGroup *group) { announce->GetSenderName(m); }

static void invAnno_subpoena_kind(mstr &m, TGroup *group)
{
	SubpoenaKind->Seek(announce->subpoena ? announce->subpoena->subpoenaKind : announce->subpoenaKind);
	m.cpy(SubpoenaKind->alias);
}

static void invAnno_session_date(mstr &m, TGroup *group) { announce->sessionDate.Print(m, "%Dг."); }
static void invAnno_subpoena_text(mstr &m, TGroup *group) { m.cpy(announce->subpoenaText); }

static void invAnno_connect_kind(mstr &m, TGroup *group)
{
	ConnectKind->Seek(announce->key.kind);
	m.cpy(ConnectKind->alias);
}

static void invAnno_lawsuit_no(mstr &m, TGroup *group) { itom(announce->key.no, m); }
static void invAnno_lawsuit_year(mstr &m, TGroup *group) { itom(announce->key.year, m); }
static void invAnno_side_no(mstr &m, TGroup *group) { itom(announce->sideNo, m); }

static void invAnno_address(mstr &m, TGroup *group)
{
	TAddress address;
	address << announce;
	address.Print(m, ADDRESS_SKIP_LN1);
}

static void invAnno_deliverer_name(mstr &m, TGroup *group)
{
	if (atob(announce->delivererUcn))
		print_user(m, announce->delivererUcn);
}

static void invAnno_text(mstr &m, TGroup *group)
{
	if (!announce->subpoenaReceived.Empty())
	{
		m.printf("%Dг. %s %s \vДата на връщане %Dг.", &announce->subpoenaReceived,
			ReceivedStatus->Seek(announce->receivedStatus), announce->receivedText, &announce->subpoenaReturned);
	}
}

#undef announce

const offield invBookAnno_fields[]=
{
	{ "F_SUBPOENA_DATE",	invAnno_subpoena_date },
	{ "F_SUBPOENA_COURT",	invAnno_subpoena_court },
	{ "F_SUBPOENA_KIND",	invAnno_subpoena_kind },
	{ "F_SESSION_DATE",	invAnno_session_date },
	{ "F_SUBPOENA_TEXT",	invAnno_subpoena_text },
	{ "F_CONNECT_KIND",	invAnno_connect_kind },
	{ "F_LAWSUIT_NO",		invAnno_lawsuit_no },
	{ "F_LAWSUIT_YEAR",	invAnno_lawsuit_year },
	{ "F_SIDE_NO",		invAnno_side_no },
	{ "F_ADDRESS",		invAnno_address },
	{ "F_DELIVERER_NAME",	invAnno_deliverer_name },
	{ "F_TEXT",			invAnno_text },
	{ NULL, NULL }
};

// ----- TInvAnnoBook --------------------------------------------------------
class TInvAnnoBook : public TBook
{
public:
	TInvAnnoBook(TWindow *parent, TInvBookAnnoGroup *group);

protected:
	const char *tr1subcourt;
	const char *tr1autogen;
	const char *tr1subkind;
	const char *tr2start;
	const char *tr2autogen;
	const char *tr2subkind;
	const char *tr2final;

	CDate lastDate;
	long lastCourt;

	long serialNoColumn;
	void Emit3(const char *start, const char *autogen, const char *subkind, const char *final, TAnnounce *group);

	virtual void Markers();
	virtual void EmitRow(TGroup *group);
	TGroup  *Process(TQuery &q);
};

TInvAnnoBook::TInvAnnoBook(TWindow *parent, TInvBookAnnoGroup *group)
	: TBook(parent, group, "InvB_Anno.htm", invBookAnno_fields, ::book_fields, BOOK_FIXED_DATES),
		serialNoColumn(group->serialNoColumn)
{
}

TGroup *TInvAnnoBook::Process(TQuery &q)
{
	TAnnounce *announce = new TAnnounce();
	*announce << q;
	return announce;
}

void TInvAnnoBook::Markers()
{
	tr1start = office.Find("tr1 start");
	tr1subcourt = office.Find("tr1 subcourt");
	tr1autogen = office.Find("tr1 autogen");
	tr1subkind = office.Find("tr1 subkind");
	tr2start = office.Find("tr2 start");
	tr2autogen = office.Find("tr2 autogen");
	tr2subkind = office.Find("tr2 subkind");
	tr2final = office.Find("tr2 final");
	tr1final = office.Find("tr1 final");
}

void TInvAnnoBook::Emit3(const char *start, const char *autogen, const char *subkind, const char *final, TAnnounce *group)
{
	office.Emit(start, autogen, invBookAnno_fields, group);

	if (serialNoColumn)
		office.Emit(autogen, subkind, invBookAnno_fields, group);

	office.Emit(subkind, final, invBookAnno_fields, group);
}

void TInvAnnoBook::EmitRow(TGroup *group)
{
	TAnnounce *announce = (TAnnounce *) group;

	if (count)
	{
		if (announce->subpoenaDate == lastDate)
		{
			if (announce->court == lastCourt)
				Emit3(tr2start, tr2autogen, tr2subkind, tr2final, announce);
			else
			{
				office.Emit(tr2final, tr1final, null_fields, NULL);
				Emit3(tr1subcourt, tr1autogen, tr1subkind, tr2start, announce);
				Emit3(tr2start, tr2autogen, tr2subkind, tr2final, announce);
			}
		}
		else
		{
			office.Emit(tr2final, tr1final, null_fields, NULL);
			Emit3(tr1start, tr1autogen, tr1subkind, tr2start, announce);
			Emit3(tr2start, tr2autogen, tr2subkind, tr2final, announce);
		}
	}
	else
	{
		Emit3(tr1start, tr1autogen, tr1subkind, tr2start, announce);
		Emit3(tr2start, tr2autogen, tr2subkind, tr2final, announce);
	}

	lastDate = announce->subpoenaDate;
	lastCourt = announce->court;
	count++;

	if (count == array->Count())
		tr1final = tr2final;
}

// ----- invAnno_book --------------------------------------------------------
#include "InvBookAnnoDialog.h"
#include "InvBookAnnoDialog.cpp"

void invAnno_book(TWindow *parent)
{
	TInvBookAnnoGroup group;

	if (TInvBookAnnoDialog(parent, &group).Execute() == IDOK)
	{
		TInvAnnoBook(parent, &group).Execute(BOOK_EXEC_HEADER);
		Street->uclp = COURT_UCLP;
	}
}
