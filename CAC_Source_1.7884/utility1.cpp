#include "cac.h"

class TRKO : public TSession
{
public:
	TRKO(TExpense *tExpense, TIndexContainer *tContainer, TSession *session, TLawsuit *tLawsuit, TSubpoena &tSubpoena);

	TIndexContainer *container;
	TExpense *expense;
	TLawsuit *lawsuit;
	TAddress address;
};

TRKO::TRKO(TExpense *tExpense, TIndexContainer *tContainer, TSession *session, TLawsuit *tLawsuit, TSubpoena &subpoena)
	: expense(tExpense), container(tContainer), lawsuit(tLawsuit)
{
	*this << session;
	address << subpoena;
}

static void print_person(mstr &m, const char *ucn, char ucnType, bool fullName)
{
	TCitizen citizen;

	citizen.ucnType = ucnType;
	strcpy(citizen.ucn, ucn);

	if (citizen.Try())
	{
		if (fullName)
			m.printf("%s %s %s", citizen.name, citizen.reName, citizen.family);
		else
			m.printf("%c.%s", *citizen.name, citizen.family);
	}
	else if (ucnType == UCN_FIRM_CODE)
	{
		TFirm firm;

		firm.ucnType = ucnType;
		strcpy(firm.ucn, ucn);

		if (firm.Try())
			m.cat(firm.name);
	}
}

#define rko OFFICE_GROUP_CAST(TRKO)
#define expense (rko->expense)

static void rko_college(mstr &m, TGroup *group) { m.cat(College->Seek(rko->lawsuit->college)); }
static void rko_composition(mstr &m, TGroup *group) { m.cat(Composition->Seek(rko->composition)); }
static void	rko_note(mstr &m, TGroup *group) { m.cat(expense->paidNote); }
static void rko_event_date(mstr &m, TGroup *group) { container_event_date(m, (TGroup *) rko->container); }
static void rko_law_kind(mstr &m, TGroup *group) { m.cat(Kind->Seek(rko->lawsuit->kind)); }

static void rko_account(mstr &m, TGroup *group)
{
	if (expense->accountType)
	{
		TAccountType account;
		account.accountType = expense->accountType;
		if (account.Try())
			m.cat(account.name);
	}
}

static void	rko_user(mstr &m, TGroup *group)
{
	print_person(m, expense->ucn, expense->ucnType, true);

	if (is_real_ucn(expense))
		m.printf(" ЕГН %s", expense->ucn);
	else
		m.cat(" ЕГН ........................");

#if 20170110
	TAddress &address = rko->address;
#else  // 20170110
	TAddress address;

	strcpy(address.ucn, expense->ucn);
	address.ucnType = expense->ucnType;

	if (address.Try())
#endif  // 20170110
	{
		address.MakeLines(ADDRESS_DISTINCT);
		m.printf(" %s %s %s, %s%s %s ", address.line1, address.line2, address.line3, address.remark,
			any(address.remark) ? "," : "", Involvement->Seek(expense->involvement));
	}
}

static void rko_sum_1(mstr &m, TGroup *group) { m.printf("%.2lf", expense->sum1); }
static void rko_sum_2(mstr &m, TGroup *group) { m.printf("%.2lf", expense->sum2); }
static void rko_sum(mstr &m, TGroup *group) { m.printf("%.2lf", expense->sum2 + expense->sum1); }
static void rko_sum_text(mstr &m, TGroup *group) { sum_in_words(expense->sum2 + expense->sum1, m); }
static void rko_judge(mstr &m, TGroup *group) { print_person(m, rko->judge, UCN_CITIZEN_UCN, false); }
static void rko_event_kind(mstr &m, TGroup *group) { m.cat(Kind->Seek(rko->container->IndexKind())); }
static void rko_judge_initials(mstr &m, TGroup *group) { print_initials(m, rko->judge, UCN_CITIZEN_UCN, true); }

#undef expense
#undef rko

static const offield rko_fields[] =
{
	{ "F_COLLEGE",         rko_college },
	{ "F_COMPOSITION",     rko_composition },
	{ "F_PAID_NOTE",       rko_note },
	{ "F_EVENT_DATE",      rko_event_date },
	{ "F_LAWSUIT_KIND",    rko_law_kind },
	{ "F_LAWSUIT_NO",      keygroup_key_no },
	{ "F_LAWSUIT_YEAR",    keygroup_key_year },
	{ "F_ACCOUNT",         rko_account },
	{ "F_RECEIVER",        rko_user },
	{ "F_SUM_1",           rko_sum_1 },
	{ "F_SUM_2",           rko_sum_2 },
	{ "F_SUM",             rko_sum },
	{ "F_SUM_TEXT",        rko_sum_text},
	{ "F_JUDGE",           rko_judge },
	{ "F_EVENT_KIND",      rko_event_kind },
	{ "F_COLLEGE_TYPE",    generic_college_type },
	{ "F_USER_INITIALS",   generic_user_initials },
	{ "F_JUDGE_INITIALS",  rko_judge_initials },
	{ NULL, NULL }
};

// 2011:148 LPR: rewritten to support subpoenaKind and write subpoenas
void print_rko(TWindow *parent, TExpense *expense, TIndexContainer *container, TSession *session, TLawsuit *lawsuit,
	char subpoenaKind)
{
	TSubpoena subpoena;
	TWaitWindow wait(parent, WAIT_MORE);

	subpoena << expense;
	subpoena.eventStatus = container->Status();
	subpoena.subpoenaKind = subpoenaKind;
	subpoena.subpoenaDate = Today;
	subpoena.officiallyNotified = SUBPOENA_OFFICIALLY_NONNOTIFIED;	// :-)
	subpoena.receivedStatus = SRS_CREATED;
	strcpy(subpoena.userUcn, Default->ucn);
	sprintf(subpoena.receivedText, "%s %.2lf", SumState->Seek(expense->sumState), expense->Sum());

	TSideWind sideWind;
	sideWind << expense;

	if (get_address(parent, subpoena, &sideWind, ADDRESS_BULGARIAN_ONLY) && subpoena.Insert())
	{
		TRKO rko(expense, container, session, lawsuit, subpoena);
		char target[SIZE_OF_PATH];
		TOffice office;

		electric_name(&subpoena, target, TYPE_TEXT);
		SubpoenaKind->Seek(subpoenaKind);
		// 2013:164 copied from printsub; 2014:239 FIX: always save
		if (office.Open(SubpoenaKind->htemplate, OFFICE_COPY_ALL, basename(target)))
		{
			office.Emit(NULL, NULL, rko_fields, &rko);
			office.ShowSync(&wait, true, "Разходния касов ордер ще бъде отворен", "Разходния касов ордер е записан");
			electric_copy(office.Name, target);
			wait.ResetForeground();
		}
	}
}

static const offield sideList_title[] =
{
	{ "F_LAWSUIT_TYPE", basewind_key_type },
	{ "F_LAWSUIT_NO", basewind_key_no },
	{ "F_LAWSUIT_YEAR", basewind_key_year },
	{ NULL, NULL }
};

class TListSideWind : public TSideWind
{
public:
	TListSideWind() { }

	int subcount;
	int serial_no;
};

#define sideList OFFICE_GROUP_CAST(TListSideWind)

static void sidelist_no(mstr &m, TGroup *group)  { m.printf("%d", sideList->serial_no); }
static void sidelist_address(mstr &m, TGroup *group) { print_address(m, sideList, 1, ADDRESS_DISTINCT | ADDRESS_REMARK); }
static void sidelist_subcount(mstr &m, TGroup *group) { subcount(m, sideList->subcount); }

#undef sideList 

static const offield sideList_fields[] =
{
	{ "F_SUBCOUNT", sidelist_subcount },
	{ "F_NO", sidelist_no },
	{ "F_INVOLVEMENT", basewind_involvement },
	{ "F_NAME", basewind_names },
	{ "F_ADDRESS", sidelist_address },
	{ NULL, NULL }
};

#define address OFFICE_GROUP_CAST(TAnyAddress)

static void sidelist_otheraddress(mstr &m, TGroup *group)
{	
	address->Print(m, ADDRESS_DISTINCT | ADDRESS_REMARK);
}

#undef address 

static const offield other_addresses[] =
{
	{ "F_ADDRESS", sidelist_otheraddress },
	{ NULL, NULL }
};

class TLinkAddresses : public TDummyGroup
{
public:
	TLinkAddresses() { }

	TLinkWind linkWind;
	TGSArray<TAnyAddress> arrayAddress;
	long no;
};

#define linkAddress OFFICE_GROUP_CAST(TLinkAddresses)

static void link_addresses(mstr &m, TGroup *group)
{
	if (!strchr(SUMMONS_FOR_SIDE_INDIRECT_INVS, linkAddress->linkWind.involvement))
	{
		print_side_links(m, &linkAddress->linkWind, false, false, NULL);
		
		if (linkAddress->arrayAddress.Count())
		{
			m.cat("\v");
			linkAddress->arrayAddress[linkAddress->no]->Print(m, ADDRESS_DISTINCT | ADDRESS_REMARK);
		}
	}
}

#undef linkAddress 

static const offield add_address[] =
{
	{ "F_ADDRESS", link_addresses },
	{ NULL, NULL }
};

void print_side_list(TWindow*, TRCDKeyContainer*, TSideWindArray &sideWinds, TGArray<TLinkWind> &linkWinds, bool all)
{
	TOffice office;
	TLinkAddresses linkAddresses;

	if (office.Open("SideList.htm") && sideWinds.Count())
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1head = office.Find("tr1 head");
		constant tr1body = office.Find("tr1 body");
		constant tr1final = office.Find("tr1 final");

		int subcount, other_address, no = 0;
		bool flag;

		office.Emit(NULL, tr1start, sideList_title, sideWinds[0]);

		for (int i = 0; i < sideWinds.Count(); i++)
		{
			if (all || !strchr(SUMMONS_FOR_SIDE_INDIRECT_INVS, sideWinds[i]->involvement))
			{
				no++;
				flag = false;
				TAnyAddress::LoadAddresses(sideWinds[i], &linkAddresses.arrayAddress);

				subcount = linkAddresses.arrayAddress.Count(); // subcount za adresi bez vryzki
				other_address = subcount;

				for (int x = 0; x < linkWinds.Count(); x++)
					if (strcmp(linkWinds[x]->ucn, sideWinds[i]->ucn) == 0  && linkWinds[x]->ucnType == sideWinds[i]->ucnType && linkWinds[x]->key == sideWinds[i]->key)
					{
						TFoundGroup ucnGroup;

						ucnGroup.ucnType = linkWinds[x]->proxyUcnType;
						strcpy(ucnGroup.ucn, linkWinds[x]->proxyUcn);

						TLinkAddresses linkAddres;
						TAnyAddress::LoadAddresses(&ucnGroup, &linkAddres.arrayAddress);

						if (linkWinds[x]->Redirected()) // za Redirected
						{
							TLinkAddresses linkAddresRedirect;
							TSideWind redirected;
							linkWinds[x]->GetRedirect(redirected);
							TAnyAddress::LoadAddresses(&redirected, &linkAddresRedirect.arrayAddress);
							if (linkAddresRedirect.arrayAddress.Count())
							{
								subcount += linkAddresRedirect.arrayAddress.Count();
							}
						}
						else if (linkAddres.arrayAddress.Count())
							subcount += linkAddres.arrayAddress.Count();
						else subcount++;

					}

				TListSideWind utilitySideWind;
				utilitySideWind << sideWinds[i];
				utilitySideWind.subcount = subcount;
				utilitySideWind.serial_no = no;

				if (other_address)
				{
					office.Emit(tr1start, tr1final, sideList_fields, &utilitySideWind);
					for (int x = 1; x < linkAddresses.arrayAddress.Count(); x++)
					{
						office.Emit(tr1start, tr1head, null_fields, NULL);
						office.Emit(tr1body, tr1final, other_addresses, linkAddresses.arrayAddress[x]);
					}
				}
				else
					office.Emit(tr1start, tr1body, sideList_fields, &utilitySideWind);

				linkAddresses.arrayAddress.Flush();
				bool hasLink = false;

				for (int x = 0; x < linkWinds.Count(); x++)
				{
					if (strcmp(linkWinds[x]->ucn, sideWinds[i]->ucn) == 0  && linkWinds[x]->ucnType == sideWinds[i]->ucnType && linkWinds[x]->key == sideWinds[i]->key )
					{
						TFoundGroup ucnGroup;

						if (linkWinds[x]->Redirected())
						{
							ucnGroup.ucnType = linkWinds[x]->redirectUcnType;
							strcpy(ucnGroup.ucn, linkWinds[x]->redirectUcn);
						}
						else
						{
							ucnGroup.ucnType = linkWinds[x]->proxyUcnType;
							strcpy(ucnGroup.ucn, linkWinds[x]->proxyUcn);
						}

						TAnyAddress::LoadAddresses(&ucnGroup, &linkAddresses.arrayAddress);
						linkAddresses.linkWind << linkWinds[x];

						for (int i = 0; i < linkAddresses.arrayAddress.Count(); i++)
						{
							linkAddresses.no = i;
							if (other_address || flag)
							{
								office.Emit(tr1start, tr1head, null_fields, NULL);
								office.Emit(tr1body, tr1final, add_address, &linkAddresses);
							}
							else
							{
								office.Emit(tr1body, tr1final, add_address, &linkAddresses);
								flag = true;
							}
						}
						if (!linkAddresses.arrayAddress.Count())
						{	
							office.Emit(tr1body, tr1final, add_address, &linkAddresses);
						}
						linkAddresses.arrayAddress.Flush();
						hasLink = true;
					}
				}
				if (!hasLink && !subcount)
					office.Emit(tr1body, tr1final, null_fields, NULL);

			}
		}
		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(NULL);
	}
}

void print_ucn_short(mstr &m, const char *ucn)
{
	if (atob(ucn))
	{
		TCitizen citizen;

		strcpy(citizen.ucn, ucn);
		citizen.ucnType = UCN_CITIZEN_UCN;

		if (citizen.Try())
		{
			if (strlen(citizen.name) <= 2)
				m.printf(" %s", citizen.name);
			else
				m.printf(" %c.", *citizen.name);

			if (any(citizen.reName))
			{
				if (strlen(citizen.reName) <= 2)
					m.printf(" %s", citizen.reName);
				else
					m.printf(" %c.", *citizen.reName);
			}

			m.printf(" %s", citizen.family);

			if (any(citizen.reFamily))
				m.printf("-%s", citizen.reFamily);
		}
		else
			print_missing(m, UCN_CITIZEN_UCN, ucn);
	}
}

const char *instance_law_kinds(long flags)
{
	switch (flags & BOOK_INSTANCES)
	{
#if COURT_TYPE == COURT_APPEAL
		case BOOK_BOTH_INST :
		case BOOK_2ND_INST :
		case BOOK_ALL_2ND_INST : return KIND_LAWSUITS;
		case BOOK_VZ_INST : return KIND_VZ_LAWSUITS;
		case BOOK_KS_INST : return KIND_KS_LAWSUITS;
#endif  // APPEAL

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
		case BOOK_BOTH_INST : return KIND_LAWSUITS;
		case BOOK_1ST_INST : return KIND_1ST_LAWSUITS;
		case BOOK_2ND_INST :
		case BOOK_ALL_2ND_INST : return KIND_2ND_LAWSUITS;
		case BOOK_VZ_INST : return KIND_VZ_LAWSUITS;
		case BOOK_KS_INST : return KIND_KS_LAWSUITS;
#endif  // ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
		case BOOK_1ST_INST :
		case BOOK_BOTH_INST : return KIND_LAWSUITS;
#endif  // REGIONAL
	}

	fatal("%d: invalid instance flags", flags);
	return NULL;
}

int session_outcomes(char result)
{
#if COURT_TYPE == COURT_REGIONAL
	// 2013:113 LRQ/IRQ
	if (result == RESULT_ALLOW_SPLIT)
		return SESSION_DECIDE;
#endif  // REGIONAL
	if (strchr(RESULT_LEFTS, result) || strchr(RESULT_HOLDS, result))
		return SESSION_LEFT;

	if (result == RESULT_STOPPED)
		return SESSION_STOPPED;

	return SESSION_OTHER;
}

int session_outcomes(TSession *session)
{
	int outcome = session_outcomes(session->result);

	if (outcome == SESSION_OTHER)
	{
		adjust_cancelaw(session);

		if (strchr(RESULT_COMPLETIZEDS, session->result))
			return strchr(RESULT_ALL_CANCELEDS, session->result) ? SESSION_CANCEL : SESSION_DECIDE;
	}

	return outcome;
}
