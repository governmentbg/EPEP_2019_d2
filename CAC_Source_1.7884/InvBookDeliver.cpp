#include "cac.h"

// 2014:213 FB2: nested selects -> join

enum
{
	BOOK_SUBPOENA	= 0x01,
	BOOK_NOTIFIEDS	= 0x02,
	BOOK_SERIAL_NO	= 0x04
};

class TInvBookDeliverGroup : public TBookGroup
{
public:
	TInvBookDeliverGroup();

	virtual void FormCriteria(msql &m);
	virtual void Title(mstr &m);

	char deliverer[SIZE_OF_UCN];
};

TInvBookDeliverGroup::TInvBookDeliverGroup()
	: TBookGroup("T_BOOK", "F_TYPE", "F_DATE", false)
{
	Add(
		new TPersonAlias("F_DELIVERER_UCN", deliverer, sizeof deliverer),
		new TCheck("F_NOTIFIEDS", &flags, BOOK_NOTIFIEDS),
		new TCheck("F_SERIAL_NO", &flags, BOOK_SERIAL_NO),
		NULL
	);
}

void TInvBookDeliverGroup::FormCriteria(msql &m)
{
	mstr fields("T.F_NO, T.F_YEAR, T.F_KIND, T.F_SUBPOENA_RECEIVED, T.F_SUBPOENA_DATE, T.F_DELIVERER_UCN, T.F_STREET, "
		"T.F_UCLP, T.F_ADR_NO, T.F_SUBPOENA_RETURNED, T.F_RECEIVED_STATUS, T.F_RECEIVED_TEXT, "
		"T.F_SUB_NO, T.F_ENTRANCE, T.F_FLOOR, T.F_APARTMENT, T.F_BLOCK_NO, T.F_RECEIVED_TIME, ");

	if (flags & BOOK_SUBPOENA)
	{
		fields.cat("T.F_DATE, T.F_TYPE, T.F_UCN_TYPE, T.F_UCN, T.F_INVOLVEMENT, T.F_SUBPOENA_NO, T.F_EVENT_STATUS, "
			"T.F_BLOCK_NO, K.F_GENERIC_KIND AS F_SUBPOENA_KIND");

		m.Begin("T_SUBPOENA T JOIN T_SUBPOENA_KIND K ON K.F_SUBPOENA_KIND = T.F_SUBPOENA_KIND", str(fields));

		internal_subpoena_uclp_lead(m);
		m.AddIsNull("K.F_GENERIC_KIND", false);
	}
	else
	{
		fields.cat("T.F_SESSION_DATE, T.F_COURT, T.F_SUBPOENA_TEXT, T.F_SUBPOENA_KIND, T.F_COMPOSITION");
		m.Begin("T_ANNOUNCE T", str(fields));
	}

	m.AddRange("T.F_SUBPOENA_DATE", minDate, maxDate);
	m.AddString("T.F_DELIVERER_UCN", deliverer);
}

void TInvBookDeliverGroup::Title(mstr &m) { book_dates(m, this); }

class TDeliverAnnounce : public TAnnounce
{
public:
	TDeliverAnnounce() { }
	long isNotifieds;
	bool operator<=(const TDeliverAnnounce &that) const;
};

bool TDeliverAnnounce::operator<=(const TDeliverAnnounce &that) const
{
	int compare = subpoenaDate.Compare(that.subpoenaDate);
	return compare ? compare < 0 : key.Kompare(that.key) <= 0;
}

#define announce OFFICE_GROUP_CAST(TDeliverAnnounce)

static void invb_deliv_subpoena_kind(mstr &m, TGroup *group)
{
	SubpoenaKind->Seek(announce->subpoenaKind);
	m.cpy(SubpoenaKind->alias);
}

static void invb_deliv_session_date(mstr &m, TGroup *group)
{
	if (strchr(SUBPOENA_KIND_GENERIC_WRITS, announce->subpoenaKind))
		announce->sessionDate.Print(m, "наср. за %D");
}

static void invb_deliv_subpoena_text(mstr &m, TGroup *group)
{
	if (!announce->subpoena)
		m.cpy(announce->subpoenaText);
}

static void invb_deliv_subpoena_date(mstr &m, TGroup *group)
{
	// 2018-05-06: "for deliver"
	//m.cpy(announce->court == COURT_CODE ? ", изготвен(а,о) на " : ", постъпил(а,о) на ");
	m.cat(", за връчване на ");
	announce->subpoenaDate.Print(m);
}

static void invb_deliv_connect_kind(mstr &m, TGroup *group)
{
	ConnectKind->Seek(announce->key.kind);
	m.cpy(ConnectKind->alias);
}

static void invb_deliv_lawsuit_no(mstr &m, TGroup *group)
{
	TRCDKey key = { announce->key.no, announce->key.year, connectKind2Type(announce->key.kind) };
	itom(flag_key_no(&key), m);
}

static void invb_deliv_lawsuit_year(mstr &m, TGroup *group) { itom(announce->key.year, m); }
static void invb_deliv_subpoena_court(mstr &m, TGroup *group) { announce->GetSenderName(m); }

static void invb_deliv_composition(mstr &m, TGroup *group)
{
	if (PRINT_COMPOSITIONS)
	{
		if (announce->subpoena)
		{
			TLawsuit lawsuit;
			lawsuit.key = announce->subpoena->key;
			// 2014:077 LPR: check for type lawsuits
			if (strchr(TYPE_LAWSUITS, lawsuit.key.type) && lawsuit.Try("F_COMPOSITION"))
				m.printf("%s с-в", Composition->Seek(lawsuit.composition));
		}
		else if (announce->composition)
			m.printf("%s с-в", Composition->Seek(announce->composition));
	}
}

static void invb_deliv_address(mstr &m, TGroup *group)
{
	TAddress address;
	address << announce;
	address.Print(m, ADDRESS_SKIP_LN1);
}

static void invb_deliv_notifieds(mstr &m, TGroup *group)
{
	if (announce->isNotifieds && announce->subpoena)
	{
		TSubpoena subpoena;
		subpoena << announce->subpoena;
		if (subpoena.Try())
			subpoena.PrintNotifieds(m, "\v");
	}
}

static void invb_deliv_text(mstr &m, TGroup *group)
{
	if (!announce->subpoenaReceived.Empty())
	{
		announce->subpoenaReceived.Print(m, "%D ");
		announce->receivedTime.Print(m, "%T ");
		m.printf("%s %s \vДата на връщане %D", ReceivedStatus->Seek(announce->receivedStatus), announce->receivedText,
			&announce->subpoenaReturned);
	}
}

#undef announce

static const offield invDeliver_fields[]=
{
	{ "F_SUBPOENA_GENERIC_KIND",	invb_deliv_subpoena_kind },
	{ "F_SESSION_DATE",		invb_deliv_session_date },
	{ "F_SUBPOENA_TEXT",		invb_deliv_subpoena_text },
	{ "F_SUBPOENA_DATE",		invb_deliv_subpoena_date },
	{ "F_CONNECT_KIND",		invb_deliv_connect_kind },
	{ "F_LAWSUIT_NO",			invb_deliv_lawsuit_no },
	{ "F_LAWSUIT_YEAR",		invb_deliv_lawsuit_year },
	{ "F_SUBPOENA_COURT",		invb_deliv_subpoena_court },
	{ "F_COMPOSITION",		invb_deliv_composition },
	{ "F_ADDRESS",			invb_deliv_address },
	{ "F_NOTIFIEDS",			invb_deliv_notifieds },
	{ "F_TEXT",				invb_deliv_text },
	{ NULL, NULL }
};

#define invGroup OFFICE_GROUP_CAST(TInvBookDeliverGroup)

static void invb_deliv_title_period(mstr &m, TGroup *group) { invGroup->Title(m); }
static void invb_deliv_title_name(mstr &m, TGroup *group) { print_user(m, invGroup->deliverer); }

#undef invGroup

static const offield invDeliverTitle_fields[] =
{
	{ "F_PERIOD", invb_deliv_title_period },
	{ "F_DELIVERER_NAME", invb_deliv_title_name },
	{ NULL, NULL }
};

class TInvDeliverBook : public TBook
{
public:
	TInvDeliverBook(TWindow *parent, TInvBookDeliverGroup *group);

	const char *hd1start;
	const char *hd1final;
	const char *sn1start;
	const char *sn1final;

	const char *hd2start;
	const char *hd2final;
	const char *tr2start;
	const char *sn2start;
	const char *sn2final;
	const char *tr2final;

	TGSArray<TDeliverAnnounce> subpoenas;
	TGSArray<TDeliverAnnounce> announces;

	virtual void Collect();
	virtual void Markers();
	virtual void EmitBody();
	TGroup *Process(TQuery &q);
};

TInvDeliverBook::TInvDeliverBook(TWindow *parent, TInvBookDeliverGroup *group)
	: TBook(parent, group, "InvB_Deliv.htm", invDeliver_fields, invDeliverTitle_fields, BOOK_FIXED_DATES)
{
}

TGroup *TInvDeliverBook::Process(TQuery &)
{
	return NULL;
}

void TInvDeliverBook::Markers()
{
	hd1start = office.Find("hd1 start");
	hd1final = office.Find("hd1 final");
	tr1start = office.Find("tr1 start");
	sn1start = office.Find("sn1 start");
	sn1final = office.Find("sn1 final");
	tr1final = office.Find("tr1 final");

	hd2start = office.Find("hd2 start");
	hd2final = office.Find("hd2 final");
	tr2start = office.Find("tr2 start");
	sn2start = office.Find("sn2 start");
	sn2final = office.Find("sn2 final");
	tr2final = office.Find("tr2 final");
}

void TInvDeliverBook::Collect()
{
	msql m;

	group->flags |= BOOK_SUBPOENA;	// use T_SUBPOENA
	group->FormCriteria(m);
	TQuery q(str(m));

	while (q.Read())
	{
		TSubpoena *subpoena = new TSubpoena;
		*subpoena << q;

		if (subpoena->HasRegards())
			delete subpoena;
		else
		{
			TDeliverAnnounce *announce = new TDeliverAnnounce;

			announce->SetSubpoena(subpoena);
			announce->isNotifieds = group->flags & BOOK_NOTIFIEDS;
			subpoenas.Add(announce);
		}
	}

	group->flags &= ~BOOK_SUBPOENA;	// use T_ANNOUNCE
	group->FormCriteria(m);

	TQuery r(str(m));
	while (r.Read())
	{
		TDeliverAnnounce *announce = new TDeliverAnnounce;

		*announce << r;
		announce->isNotifieds = group->flags & BOOK_NOTIFIEDS;

		if (announce->court == COURT_CODE)
			subpoenas.Add(announce);
		else
			announces.Add(announce);
	}

	array->Add(new TDeliverAnnounce);
}

void TInvDeliverBook::EmitBody()
{
	office.Emit(NULL, hd1start, book_fields, group);

	if (group->flags & BOOK_SERIAL_NO)
		office.Emit(hd1start, hd1final, null_fields, NULL);

	office.Emit(hd1final, tr1start, null_fields, NULL);

	for (int i = 0; i < subpoenas.Count(); i++)
	{
		office.Emit(tr1start, sn1start, null_fields, NULL);

		if (group->flags & BOOK_SERIAL_NO)
			office.Emit(sn1start, sn1final, body_fields, subpoenas[i]);

		office.Emit(sn1final, tr1final, body_fields, subpoenas[i]);
	}

	office.Emit(tr1final, hd2start, book_fields, group);

	if (group->flags & BOOK_SERIAL_NO)
		office.Emit(hd2start, hd2final, null_fields, NULL);

	office.Emit(hd2final, tr2start, null_fields, NULL);

	for (int i = 0; i < announces.Count(); i++)
	{
		office.Emit(tr2start, sn2start, null_fields, NULL);

		if (group->flags & BOOK_SERIAL_NO)
			office.Emit(sn2start, sn2final, body_fields, announces[i]);

		office.Emit(sn2final, tr2final, body_fields, announces[i]);
	}

	office.Emit(tr2final, NULL, book_fields, group);
}

// ----- functions -----------------------------------------------------------
void invDeliver_book(TWindow *parent, TInvBookDeliverGroup *group)
{
	TInvDeliverBook(parent, group).Execute(BOOK_EXEC_HEADER);
	Street->uclp = COURT_UCLP;  // ??
}

#include "InvBookDeliverDialog.h"
#include "InvBookDeliverDialog.cpp"

void invDeliver_book(TWindow *parent)
{
	TInvBookDeliverGroup group;

	if (TInvBookDeliverDialog(parent, &group).Execute() == IDOK)
		invDeliver_book(parent, &group);
}
