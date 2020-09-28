#include "cac.h"

// ----- SUBPOENA_LIST_ ------------------------------------------------------
enum
{
	SUBPOENA_LIST_NOTIFIED	= 1,
	SUBPOENA_LIST_IMPORTANT	= 2,	// notified + will be notified
	SUBPOENA_LIST_ALL		= 3,

	SUBPOENA_LIST_SUMMARY	= 0x01,
	SUBPOENA_LIST_LETTERS	= 0x02
};

// ----- TSubpoenaListGroup --------------------------------------------------
class TSubpoenaListGroup : public TDummyGroup
{
public:
	TSubpoenaListGroup();

	long list;
	long emit;
	long empty;
};

TSubpoenaListGroup::TSubpoenaListGroup()
{
	Add(
		new TRadio("F_NOTIFIED", &list, SUBPOENA_LIST_NOTIFIED),
		new TRadio("F_IMPORTANT", &list, SUBPOENA_LIST_IMPORTANT),
		new TRadio("F_ALL", &list, SUBPOENA_LIST_ALL),
		new TCheck("F_SUMMARY", &emit, SUBPOENA_LIST_SUMMARY),
		new TCheck("F_LETTERS", &emit, SUBPOENA_LIST_LETTERS),
		new TLong("F_EMPTY", &empty),
		NULL
	);

	list = SUBPOENA_LIST_NOTIFIED;
	emit = SUBPOENA_LIST_LETTERS;
}

#include "SubpoenaListDialog.h"
#include "SubpoenaListDialog.cpp"

// ----- TSubpoenaPart -------------------------------------------------------
class TSubpoenaPart : public TSubpoena
{
public:
	TSubpoenaPart();
	void FillColumns();

	char alias[SIZE_OF_ALIAS];
	mstr name;
	mstr address;
	int subcount;
	bool dated;
	mstr text;
};

TSubpoenaPart::TSubpoenaPart()
	: subcount(0), dated(false)
{
	*alias = '\0';
}

void TSubpoenaPart::FillColumns()
{
	mstr allNamesTxt;
	mstr addressTxt;

	PrintAddress(addressTxt, ADDRESS_DISTINCT | ADDRESS_REMARK);
	print_side_links(allNamesTxt, this, true);

	if (subpoenaKind != SUBPOENA_KIND_NONE && ProxyPresent())
	{
		char *description = ncstr(allNamesTxt);
		char *proxyDescription = strchr(description, BONEVA_SIDES_DELIMITER);
		char *redirDescription = "";
		bool redirected = RedirectPresent();

		if (proxyDescription)
		{
			*proxyDescription++ = '\0';
			while (isspace(*proxyDescription))
				proxyDescription++;

			if (redirected && (redirDescription = strchr(proxyDescription, BONEVA_SIDES_DELIMITER)) != NULL)
			{
				*redirDescription++ = '\0';
				while (isspace(*redirDescription))
					redirDescription++;
			}
		}
		else
			proxyDescription = "";

		TSideWind proxy;
		GetProxy(proxy);
		bool bonevaProxyInvolvement = strchr(PERSLST_BONEVA_0_INVS, proxy.involvement) != NULL;

		name.sep_cat(" ", description);
		if (!bonevaProxyInvolvement)
			name.sep_cat(" ", proxyDescription);

		if (strchr(SUMMONS_FOR_SIDE_LEGAL_SPOK_INVS, proxyInvolvement) &&	proxyKind == SIDE_MARK_CONSENT_OF_INDIRECT)
		{
			if (!redirected)
				address.sep_cat(" ", addressTxt);
			if (bonevaProxyInvolvement)
				address.sep_cat(" ", proxyDescription);
		}
		else
		{
			if (bonevaProxyInvolvement)
				address.sep_cat(" ", proxyDescription);
			if (!redirected)
				address.sep_cat(" ", addressTxt);
		}

		if (redirected)
		{
			TSideWind redirect;
			GetRedirect(redirect);
			bool bonevaRedirectInvolvement = strchr(PERSLST_BONEVA_0_INVS, redirect.involvement);

			if (!bonevaRedirectInvolvement)
				name.sep_cat(" ", redirDescription);

			if (bonevaRedirectInvolvement)
				address.sep_cat(" ", redirDescription);
			address.sep_cat(" ", addressTxt);
		}
	}
	else
	{
		name.cpy(allNamesTxt);
		address.cpy(addressTxt);
	}
}

#define subpart OFFICE_GROUP_CAST(TSubpoenaPart)

static void subpart_subcount(mstr &m, TGroup *group) { itom(subpart->subcount, m); }
static void subpart_name(mstr &m, TGroup *group) { m.cpy(subpart->name); }
static void subpart_address(mstr &m, TGroup *group) { m.cpy(subpart->address); }

static void subpart_remark(mstr &m, TGroup *group)
{
	char subpoenaKind = subpart->subpoenaKind;
	SubpoenaKind->Seek(subpoenaKind);

	if (SubpoenaKind->proc != SK_PROC_STANDALONE_LETTER)
		m.cpy(subpart->alias);
	// non-zero test is just in case
	if (subpart->multi && subpart->multi != SUBPOENA_SINGLE)
		m.cat(" - обща(о)");
	m.cat(", ");
	print_initials(m, subpart->userUcn, UCN_CITIZEN_UCN, true);
	m.cat("\v");

	if (subpoenaKind == SUBPOENA_KIND_NOTIFY_EMAIL)
		m.cat(subpart->email);
	else if (subpoenaKind == SUBPOENA_KIND_NOTIFY_PHONE)
		m.cat(subpart->phone);

	// from SubpoenaListColumns::MakeColumns()
	// данни за призовката; 2009:327 LPR: rewrite
	if (subpart->Received())
	{
		ReceivedStatus->Seek(subpart->receivedStatus);
		m.cat(ReceivedStatus->alias);

		// N.B. 47/51 checks do not match TSubpoena::PrintDescription()
		// 2013:336 LPR: was separate check for SRS 47 and 51
		if (!strchr(SRS_FORTY_FIFTY, subpart->receivedStatus))
			subpart->subpoenaReceived.Print(m, " на %Dг. ");
	}

	if (any(subpart->text))
		m.printf(" %s", str(subpart->text));
}

static void subpart_date(mstr &m, TGroup *group)
{
	if (subpart->dated)
		subpart->subpoenaDate.Print(m, C_DATE_GOD);
}

#undef subpart

static const offield subpart_fields[] =
{
	{ "F_SUBCOUNT",	subpart_subcount },
	{ "F_NAME",		subpart_name },
	{ "F_ADDRESS",	subpart_address },
	{ "F_REMARK",	subpart_remark },
	{ "F_DATE",		subpart_date },
	{ NULL,		NULL }
};

// ----- TSubpoenaSide -------------------------------------------------------
class TSubpoenaSide : public TSideWind
{
public:
	TSubpoenaSide() { }

	int serialNo;
	TGArray<TSubpoenaPart> subparts;

	void PostProcess();
};

#define subside OFFICE_GROUP_CAST(TSubpoenaSide)

static void subside_subcount(mstr &m, TGroup *group) { itom(subside->subparts.Count(), m); }
static void subside_serial_no(mstr &m, TGroup *group) { itom(subside->serialNo, m); }

static void subside_involvement(mstr &m, TGroup *group)
{
	basewind_involvement(m, group);

	if (subside->fromSide != INVOLVEMENT_FROM_NULL)
		m.printf(" от страна на %s", Involvement->Seek(subside->fromSide));
}

#undef subside

static const offield subside_fields[] =
{
	{ "F_SUBCOUNT",		subside_subcount },
	{ "F_SERIAL_NO",		subside_serial_no },
	{ "F_INVOLVEMENT",	subside_involvement },
	{ NULL,			NULL }
};

// from SubpoenaList::Load(), cut commented uclp code; 2010:007 LPR: rewrite
static void FillText(const TSubpoena *subp, mstr &m)
{
	if (any(m))
		m.cat("\v");

	SubpoenaKind->Seek(subp->subpoenaKind);
	if (SubpoenaKind->ShowSubpoenaDate())
	{
		m.printf(" %s", SubpoenaKind->alias);
		ReceivedStatus->Seek(subp->receivedStatus);
		if (ReceivedStatus->ShowSubpoenaDate())
		{
			m.printf(", %s ", ReceivedStatus->alias);
			subp->PrintSentDescr(m);
		}
	}

	if (atof(subp->lReceiverUcn))
	{
		char ra[SIZE_OF_NAME_FOR_BOOK];
		if (ucnAndUcnTypeToName(subp->lReceiverUcn, subp->lReceiverUcnType, ra, sizeof ra))
			m.printf(" До %s ", ra);
	}
	else
		m.printf(" %s ", subp->receivedText);
}

// ----- TSubpoenaList -------------------------------------------------------
class TSubpoenaList : public TProcess
{
public:
	TSubpoenaList(TWindow *parent, TRCDKeyContainer *tContainer, TGArray<TSideWind> *tSideWinds,
		TSubpoenaListGroup *tGroup);

	TRCDKeyContainer *container;
	mstr summary;

	void Show();

protected:
	TSubpoenaListGroup *group;
	TGArray<TSubpoenaSide> subsides;
	TGArray<TSubpoena> letters;

	void PostProcess(TSubpoenaSide *subside);
};

TSubpoenaList::TSubpoenaList(TWindow *parent, TRCDKeyContainer *tContainer, TGArray<TSideWind> *sideWinds,
	TSubpoenaListGroup *tGroup)
	: TProcess(parent, "списъка", SAVE_SUBPOENAS ? WAIT_MORE : WAIT), container(tContainer), group(tGroup)
{
	for (int i = 0; i < sideWinds->Count(); i++)
	{
		TSubpoenaSide *subside = new TSubpoenaSide;
		*subside << (*sideWinds)[i];
		subsides.Add(subside);
	}

	// add supoenas
	TSubpoenaPart *subpart = new TSubpoenaPart;
	msql m;

	*subpart << container;
	subpart->eventStatus = container->Status();
	subpart->FloatCriteria(m);
	m.AddOrder("F_SUBPOENA_NO");

	TQuery q(str(m));
	while (q.Read())
	{
		*subpart << q;
		SubpoenaKind->Seek(subpart->subpoenaKind);
		// 2011:157 LRQ: reject proc other
		if (SubpoenaKind->proc == SK_PROC_OTHER ||
			(subpart->subpoenaKind == SK_WILL_BE_SUMMONED && group->list == SUBPOENA_LIST_NOTIFIED))
			continue;

		strcpy(subpart->alias, SubpoenaKind->alias);

		if (SubpoenaKind->proc != SK_PROC_STANDALONE_LETTER)
			subpart->text.cpy(subpart->receivedText);

		if (SubpoenaKind->proc == SK_PROC_LETTER_TO)
		{
			if (group->emit & SUBPOENA_LIST_LETTERS)
				letters.Add(subpart);
		}
		else
		{
			int i;

			if (SubpoenaKind->ShowSubpoenaDate())
			{
				ReceivedStatus->Seek(subpart->receivedStatus);
				if (ReceivedStatus->ShowSubpoenaDate())
					subpart->dated = true;
			}

			for (i = 0; i < subsides.Count(); i++)
			{
				// 2008:333 FIX: +involvement
				if (
					subsides[i]->ucnType == subpart->ucnType &&
					!strcmp(subsides[i]->ucn, subpart->ucn) &&
					subsides[i]->involvement == subpart->involvement)
				{
					if (SubpoenaKind->proc == SK_PROC_STANDALONE_LETTER)
						FillText(subpart, subpart->text);
					subsides[i]->subparts.Add(subpart);
					break;
				}
			}

			if (i == subsides.Count())
			{
				error("Призованото или уведомено лице %s %s %s не фигурира в списъка на страните.\n",
					Involvement->Seek(subpart->involvement), UCNType->Seek(subpart->ucnType), subpart->ucn);
				delete subpart;
			}
		}
		subpart = new TSubpoenaPart;
	}

	delete subpart;
	// post-process subsides
	long serialNo = 1;

	for (int i = 0; i < subsides.Count(); i++)
	{
		TSubpoenaSide *subside = subsides[i];

		if (!subside->subparts.Count())
		{
			if (group->list != SUBPOENA_LIST_ALL)
			{
				subsides.Remove(i--);
				continue;
			}

			TSubpoenaPart *subpart = new TSubpoenaPart;

			subpart->SetOrdinary(subside);
			//subpart->subpoenaDate.Clear();
			subside->subparts.Add(subpart);
		}

		subside->serialNo = serialNo++;
		PostProcess(subsides[i]);
	}

	while (letters.Count())
	{
		TSubpoena *letter = letters[0];
		dprintf("Придружително писмо %ld за %s %s е към липсваща призовка/съобщение.\n", letter->subpoenaNo, UCNType->Seek(letter->ucnType), letter->ucn);
		letters.Remove(0);
	}

	for (int i = 0; i < group->empty; i++)
	{
		TSubpoenaSide *subside = new TSubpoenaSide;
		subside->serialNo = serialNo++;
		subside->involvement = INVOLVEMENT_INTERNAL;
		subsides.Add(subside);
		TSubpoenaPart *subpart = new TSubpoenaPart;
		subpart->subcount++;
		subside->subparts.Add(subpart);
	}
}

void TSubpoenaList::PostProcess(TSubpoenaSide *subside)
{
	TGArray<TSubpoenaPart> *subparts = &subside->subparts;
	char side[SIZE_OF_SUBPART_COLUMN];
	sprintf(side, "%d: ", subside->serialNo);
	char *end = side + strlen(side);
	TSubpoenaPart *last = NULL;

	for (int i = 0; i < subparts->Count(); i++)
	{
		TSubpoenaPart *subpart = (*subparts)[i];

		subpart->FillColumns();
		subpart->name.trim_all();
		if (!last || subpart->name.cmp(last->name))
			last = subpart;
		last->subcount++;

		if ((group->emit & SUBPOENA_LIST_SUMMARY) && subpart->dated)
		{
			mstr m;
			subpart->subpoenaDate.Print(m);
			strcatell(end, str(m), ", ", sizeof side);
		}

		// 2010:011 LPR: modified to handle 2+ letters for a subpart
		int i = 0;
		while (i < letters.Count())
		{
			TSubpoena *letter = letters[i];

			if (
				letter->regard == subpart->subpoenaNo &&
				letter->involvement == subpart->involvement &&
				letter->ucnType == subpart->ucnType &&
				!strcmp(letter->ucn, subpart->ucn)
			)
			{
				FillText(letter, subpart->text);
				letters.Remove(i);
			}
			else
				i++;
		}
	}

	if (*end)
	{
		summary.sep("; ");
		summary.cat(side);
	}
}

#define sublist OFFICE_GROUP_CAST(TSubpoenaList)

static void sublist_summary(mstr &m, TGroup *group) { m.cpy(sublist->summary); }

static void sublist_title(mstr &m, TGroup *group)
{
	mstr sOrigin;
	mstr sDate;
	TRCDKeyContainer *container = sublist->container;
	char status = container->Status();
	TRCDKey origin;

	if (!find_origin(container, origin))
		origin = container->key;

	const char *name = Type->Seek(origin.type);

	if (strchr(TYPE_LAWSUITS, origin.type))
	{
		TLawsuit lawsuit;
		lawsuit.key = origin;

		if (lawsuit.Try("F_KIND"))
		{
			Kind->Seek(lawsuit.kind);
			name = Kind->alias;
		}
	}

	sOrigin.printf("%s %ld/%ldг.", name, flag_key_no(&origin), origin.year);
	container->date.Print(sDate, "%Dг.");

	if (strchr(KIND_SESSIONS, container->kind) && status == RESULT_FIXED)
	{
		const TSession *session = (const TSession *) container;

		m.printf("на лицата, призовани по %s, насрочено за %s, %d.%02d часа",
			str(sOrigin), str(sDate), session->time.hour, session->time.minute);

		if (session->end > session->date)
		{
			m.printf(" до %Dг.", &session->end);
			session->entime.Print(m, ", %T часа");
		}
	}
	else
	{
		TDecision tDecision;
		TLong *eventNo = (TLong *) container->Find("F_EVENT_NO");

		// 2017:010 IRQ/URQ: sessions with eventNo equals container
		if (strchr(KIND_SESSIONS, container->kind) && eventNo->L())
		{
			const TSession *session = (const TSession *) container;

			tDecision.kind = session->IndexKind();
			tDecision.eventNo = session->eventNo;
			tDecision.fromDate = session->date;
			tDecision.sessionKind = session->kind;
			container = &tDecision;
		}

		m.printf("на лицата с изготвени съобщения или уведомени\v" "по %s\v" "%s", str(sOrigin),
			Kind->Seek(container->kind));

		// 2008:303 FIX: if sessions -> decisions or else
		// 2017:003 IRQ/FIX? decisions only -> all endocs
		if (strchr(KIND_ENDOCS, container->kind))
		{
			const TDecision *decision = (const TDecision *) container;

			if (PRINT_JUDGEMENTS)
				m.printf(" № %ld/%ld", decision->eventNo, decision->fromDate.year);

			m.printf(" (%s от %s - %s)", Kind->Seek(decision->sessionKind), str(sDate), Result->Seek(status));
		}
		else
		{
			// 2016:354 LPR/IRQ: resolutions, PRINT_JUDGEMENTS
			if (strchr(KIND_INREG_EVENTS, container->kind) || strchr(KIND_REQUEST_RESOLUTIONS, container->kind))
			{
				if (PRINT_JUDGEMENTS && eventNo->L())
					m.printf(" № %ld", eventNo->L());
			}
			// 2008:352 FIX: resolution seek; 2017:010 rrseek
			m.printf(" от %s - %s", str(sDate), rrseek(container, status));
				//container->kind == KIND_REQUEST_RESOLUTION ? Resolution->Seek(status) : Result->Seek(status));
		}
	}
}

#undef sublist

static const offield sublist_fields[] =
{
	{ "F_SUMMARY",	sublist_summary },
	{ "F_TITLE",	sublist_title },
	{ NULL,		NULL }
};

void TSubpoenaList::Show()
{
	TOffice office;
	char target[SIZE_OF_PATH];

	electric_name(container, target, TYPE_TEXT, container->Status() == RESULT_FIXED ? EXTRA_NO_SUBPLIST_FIXED :
		EXTRA_NO_SUBPLIST_OTHER);

	if (office.Open("SubpList.htm", OFFICE_COPY_ALL, SAVE_SUBPOENAS ? basename(target) : NULL))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1serial = office.Find("tr1 serial");
		constant tr1name = office.Find("tr1 name");
		constant tr1address = office.Find("tr1 address");
		constant tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, sublist_fields, this);

		for (int i = 0; i < subsides.Count(); i++)
		{
			TSubpoenaSide *subside = subsides[i];

			for (int i = 0; i < subside->subparts.Count(); i++)
			{
				office.Emit(tr1start, i ? tr1serial : tr1name, subside_fields, subside);
				office.Emit(subside->subparts[i]->subcount ? tr1name : tr1address, tr1final, subpart_fields, subside->subparts[i]);
			}
		}

		office.Emit(tr1final, NULL, sublist_fields, this);
		office.ShowSync(wait, SAVE_SUBPOENAS, "Списъкът ще бъде отворен", "Списъкът е записан");

		if (SAVE_SUBPOENAS)
		{
			electric_copy(office.Name, target);
			wait->ResetForeground();
		}
	}
}

void print_subpoena_list(TWindow *parent, TRCDKeyContainer *container, TGArray<TSideWind> *sideWinds)
{
	TSubpoenaListGroup group;

	if (container->Status() != RESULT_FIXED || TSubpoenaListDialog(parent, &group).Execute() == IDOK)
		TSubpoenaList(parent, container, sideWinds, &group).Show();
}
