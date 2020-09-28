#include "cac.h"

// ----- TPrintOutParams -----------------------------------------------------
struct TPrintOutParams
{
	const TOutReg *outReg;
	TInReg *inReg;
	TRCDKeyContainer *event;
	TGArray<TInRegEvent> *inRegEvents;
	const TConnect *lower;
	TTransGroup *lReceiver;
};

// ----- TPrintOutReg --------------------------------------------------------
class TPrintOutReg : public TOutReg
{
public:
	TPrintOutReg(TPrintOutParams &tp, char tSubpoenaKind, TAddress *tLReceiverAddress);

	bool LReceiverNotice() const { return subpoenaKind == SUBPOENA_KIND_DELIVERY_NOTICE && p.lReceiver; }

	char originKind;
	long composition;
	char eispp_nmr[SIZE_OF_EISPP_NMR];
	long cutDown;
	char judge[SIZE_OF_UCN];

	TInReg bInReg;
	long bCount;
	bool bFirst;
	TPrintOutParams &p;
	const char subpoenaKind;
	TAddress *lReceiverAddress;
};

TPrintOutReg::TPrintOutReg(TPrintOutParams &tp, char tSubpoenaKind, TAddress *tLReceiverAddress)
	: p(tp), subpoenaKind(tSubpoenaKind), lReceiverAddress(tLReceiverAddress),
		originKind('\0'), composition(0), bCount(0), bFirst(true)
{
	*this << p.outReg;

	if (lawsuit.type)
	{
		if (strchr(TYPE_LAWSUITS, lawsuit.type))
		{
			TLawsuit tLawsuit;
			tLawsuit.key = lawsuit;

			if (tLawsuit.Try("F_KIND, F_COMPOSITION, F_EISPP_NMR, F_CUTDOWN, F_JUDGE"))
			{
				originKind = tLawsuit.kind;
				composition = tLawsuit.composition;
				strcpy(eispp_nmr, tLawsuit.eispp_nmr);
				cutDown = tLawsuit.cutDown;
				strcpy(judge, tLawsuit.judge);
			}
		}
		else if (lawsuit.type == TYPE_REQUEST)
		{
			TRequest request;
			request.key = lawsuit;

			if (request.Try("F_KIND, F_EISPP_NMR"))
			{
				originKind = request.kind;
				composition = '\0';
				strcpy(eispp_nmr, request.eispp_nmr);
				cutDown = 0;
			}
		}
	}
}

static void print_inreg_kind(mstr &m, TInReg *inReg)
{
	m.cat(inReg->kind == KIND_OTHER_IN_DOCUMENT && any(inReg->text) ? inReg->text : Kind->Seek(inReg->kind));
}

void print_inreg_appealers(mstr &m, TInReg *inReg)
{
	TGArray<TSideWind> sideWinds;
	bool first = true;

	load_sidebasicv(inReg, sideWinds);
	for (int i = 0; i < sideWinds.Count(); i++)
	{
		if (sideWinds[i]->Appealer())
		{
			add_side_name(m, sideWinds[i], first ? "" : NULL);
			first = false;
		}
	}
}

static void print_inreg_no_date(mstr &m, TInReg *inReg) { m.printf("%ld/%Dг.", inReg->key.no, &inReg->date); }

static void print_single_inreg(mstr &m, TInReg *inReg)
{
	if (any(m))
		m.cat("; ");

	print_inreg_kind(m, inReg);
	m.cat(" на ");
	print_inreg_appealers(m, inReg);
	m.cat(" ");
	print_inreg_no_date(m, inReg);
}

static void print_regard_addr(mstr &m, const TPrintOutReg *outReg, bool uclp)
{
	TAddress *address = outReg->lReceiverAddress;
	TAddress tAddress;

	if (!address && atob(outReg->receiver))
	{
		strcpy(tAddress.ucn, outReg->receiver);
		tAddress.ucnType = receiver_ucn_type(outReg->receiverType);

		if (tAddress.Try(uclp ? "F_UCLP" : NULL))
			address = &tAddress;
	}

	if (address)
	{
		if (uclp)
			m.cat(UCLP->Seek(address->uclp));
		else	// or SKIP_LINE_1?..
			address->Print(m, ADDRESS_DISTINCT | ADDRESS_REMARK | ADDRESS_SKIP_UCLP);
	}
}

#define printout OFFICE_GROUP_CAST(TPrintOutReg)

static void printout_college(mstr &m, TGroup *group) { print_college(m, printout->college); }
static void printout_composition(mstr &m, TGroup *group) { print_composition(m, printout->composition); }
static void printout_composition_name(mstr &m, TGroup *group) { print_composition(m, printout->composition, ""); }

static void printout_lawsuit_kind(mstr &m, TGroup *group)
{
	if (printout->originKind)
		m.cpy(Kind->Seek(printout->originKind));
}

static void printout_lawsuit_no(mstr &m, TGroup *group) { itom(printout->lawsuit.no, m); }
static void printout_lawsuit_year(mstr &m, TGroup *group) { itom(printout->lawsuit.year, m); }

static void printout_lreceiver(mstr &m, TGroup *group)
{
	TUCNGroup *ucnGroup = TNamedGroup::NewGroup(printout->p.lReceiver->ucnType);
	TVirtPtr VP(ucnGroup);

	*ucnGroup << printout->p.lReceiver;
	ucnGroup->Print(m);
	m.delead(SIZE_OF_UCN_TYPE_UCN);
}

static void printout_lreceiver_address(mstr &m, TGroup *group)
{
	printout->lReceiverAddress->Print(m, ADDRESS_DISTINCT | ADDRESS_REMARK);
}

static void printout_receiver(mstr &m, TGroup *group)
{
	if (atob(printout->receiver))
		print_sender(m, printout->receiverType, printout->receiver);
	else if (printout->LReceiverNotice())
		printout_lreceiver(m, group);
}

static void printout_receiver_address(mstr &m, TGroup *group)
{
	TSender sender;

	sender.ucnType = printout->receiverType;
	strcpy(sender.ucn, printout->receiver);
	print_address(m, &sender, 1, ADDRESS_DISTINCT | ADDRESS_REMARK);
}

static void printout_inreg_docs(mstr &m, TGroup *group)
{
	if (printout->key.no)
	{
		TGArray<TInRegEvent> events;
		TInReg inReg;

		printout->LoadInRegs(&events, "F_NO, F_YEAR", "F_KIND, F_DATE");

		for (int i = 0; i < events.Count(); i++)
		{
			inReg << events[i];
			print_single_inreg(m, &inReg);
		}
	}
	else if (printout->p.inReg)
		print_single_inreg(m, printout->p.inReg);
}

static void printout_event_kind(mstr &m, TGroup *group)
{
	char kind = '\0';

	if (printout->p.inReg)
		kind = printout->p.inReg->decisionKind;
	else if (printout->p.event)
		kind = kind2VisibleKind(printout->p.event->kind);

	if (kind)
		m.cat(Kind->Seek(kind));
}

static void printout_event_no(mstr &m, TGroup *group)
{
	if (printout->p.event)
		container_event_no(m, printout->p.event);
}

static void printout_event_date(mstr &m, TGroup *group)
{
	CDate date;

	if (printout->p.inReg)
	{
		TDecision decision;

		decision.key = printout->p.inReg->lawsuit;
		decision.kind = printout->p.inReg->decisionKind;
		decision.date = printout->p.inReg->decisionDate;
		date = decision.Try("F_FROM_DATE") ? decision.fromDate : printout->p.inReg->decisionDate;
	}
	else if (printout->p.event)
		date = container_event_date1(printout->p.event);

	if (!date.Empty())
		date.Print(m);
}

static void printout_lower_outreg_no(mstr &m, TGroup *group) { itom(printout->p.lower->outNo, m); }
static void printout_lower_outreg_year(mstr &m, TGroup *group) { itom(printout->p.lower->outYear, m); }

static void printout_lower_kind(mstr &m, TGroup *group)
{
	if (printout->p.lower->connectKind)
		m.cpy(ConnectKind->Seek(printout->p.lower->connectKind));
}

static void printout_lower_no(mstr &m, TGroup *group) { m.cpy(printout->p.lower->connectNo); }
static void printout_lower_year(mstr &m, TGroup *group) { itom(printout->p.lower->connectYear, m); }

static void printout_juriconf(mstr &m, TGroup *group)
{
	m.cpy(printout->receiverType == UCN_INSTANCE_EQUAL ? "подсъдност" : "компетентност");
}

static void printout_connects(mstr &m, TGroup *group) { print_connects(m, printout->lawsuit, false, true, NULL); }

static void printout_eispp_nmr(mstr &m, TGroup *group)
{
	if (any(printout->eispp_nmr))
		m.printf("Номер по ЕИСПП на НП %s", printout->eispp_nmr);
}

static void printout_cutdown(mstr &m, TGroup *group)
{
	if (printout->cutDown == LAWSUIT_CUTDOWN_P1 || printout->cutDown == LAWSUIT_CUTDOWN_P2)
		m.printf("Съкр.производство чл.371, т.%d НПК", printout->cutDown == LAWSUIT_CUTDOWN_P1 ? 1 : 2);
}

static void printout_judge(mstr &m, TGroup *group)
{
	if (atob(printout->judge))
		print_user(m, printout->judge, UCN_CITIZEN_UCN, true, -1);
}

static void printout_inreg_kind(mstr &m, TGroup *group)
{
	if (printout->bFirst)
		printout->bFirst = false;
	else
		m.cat("\v");

	print_inreg_kind(m, &printout->bInReg);
}

static void printout_inreg_count(mstr &m, TGroup *group) { itom(printout->bCount, m); }
static void printout_inreg_no_date(mstr &m, TGroup *group) { print_inreg_no_date(m, &printout->bInReg); }
static void printout_inreg_appealers(mstr &m, TGroup *group) { print_inreg_appealers(m, &printout->bInReg); }
static void printout_college_initials(mstr &m, TGroup *group) { print_college_initials(m, printout->college); }

static void printout_judge_initials(mstr &m, TGroup *group)
{
	if (printout->p.inReg)
	{
		// query_events is heavy
		TInRegEvent inRegEvent;
		msql w(inRegEvent.Name, "F_JUDGE");

		w.Add(printout->p.inReg, "F_NO, F_YEAR, F_TYPE");
		w.AddIsNull("F_JUDGE", false);
		w.Add("F_DATE");
		w.printf("<= %ld", printout->date.Value());
		w.AddOrder("F_DATE DESC");

		TQuery q(str(w));
		if (q.Read())
		{
			inRegEvent << q;
			print_initials(m, inRegEvent.judge, UCN_CITIZEN_UCN, true);
			return;
		}
	}

	if (printout->lawsuit.type && strchr(TYPE_LAWSUITS, printout->lawsuit.type))
	{
		TLawsuit lawsuit;
		lawsuit.key = printout->lawsuit;

		if (lawsuit.Get("F_JUDGE"))
			print_initials(m, lawsuit.judge, UCN_CITIZEN_UCN, true);
	}
}

static void printout_judge_name(mstr &m, TGroup *group)
{
	if (atob(printout->judge))
	{
		TCitizen citizen;

		citizen.ucnType = UCN_CITIZEN_UCN;
		strcpy(citizen.ucn, printout->judge);

		if (citizen.Try())
		{
			#if COURT_TYPE == COURT_MILITARY
				m.cat(citizen.Rank());
			#endif  // MILITARY
			m.printf("%s %s", citizen.name, citizen.family);
		}
	}
}

static void printout_regard_addr_skip_uclp(mstr &m, TGroup *group) { print_regard_addr(m, printout, false); }
static void printout_regard_addr_uclp_only(mstr &m, TGroup *group) { print_regard_addr(m, printout, true); }

#undef printout

static const offield printout_fields[] =
{
	{ "F_JUDGE_TYPE",		generic_judge_type },
	{ "F_JUDGE_TYPE_COURT",	generic_judge_type_court },
	{ "F_COLLEGE",		printout_college },
	{ "F_COMPOSITION",	printout_composition },
	{ "F_LAWSUIT_KIND",	printout_lawsuit_kind },
	{ "F_LAWSUIT_NO",		printout_lawsuit_no },
	{ "F_LAWSUIT_YEAR",	printout_lawsuit_year },
	{ "F_RECEIVER",		printout_receiver },
	{ "F_RECEIVER_ADDRESS",	printout_receiver_address },
	{ "F_OUTREG_NO",		keygroup_key_no },
	{ "F_OUTREG_DATE",	container_date },
	{ "F_INREG_DOCS",		printout_inreg_docs },
	{ "F_EVENT_KIND",		printout_event_kind },
	{ "F_EVENT_NO",		printout_event_no },
	{ "F_EVENT_DATE",		printout_event_date },
	{ "F_LOWER_OUTREG_NO",	printout_lower_outreg_no },
	{ "F_LOWER_OUTREG_YEAR",printout_lower_outreg_year },
	{ "F_LOWER_KIND",		printout_lower_kind },
	{ "F_LOWER_NO",		printout_lower_no },
	{ "F_LOWER_YEAR",		printout_lower_year },
	{ "F_JURICONF",		printout_juriconf },
	{ "F_CONNECTS",		printout_connects },
	{ "F_EISPP_NMR",		printout_eispp_nmr },
	{ "F_CUTDOWN",		printout_cutdown },
	{ "F_JUDGE",		printout_judge },
	{ "F_INREG_KIND",		printout_inreg_kind },
	{ "F_INREG_COUNT",	printout_inreg_count },
	{ "F_INREG_NO_DATE",	printout_inreg_no_date },
	{ "F_INREG_APPEALERS",	printout_inreg_appealers },
	{ "F_LRECEIVER",		printout_lreceiver },
	{ "F_LRECEIVER_ADDRESS",printout_lreceiver_address },
	{ "F_COMPOSITION_NAME",	printout_composition_name },
	{ "F_USER_INITIALS",    generic_user_initials },
	{ "F_COLLEGE_INITIALS",	printout_college_initials },
	{ "F_JUDGE_INITIALS",	printout_judge_initials },
	{ "F_JUDGE_NAME",		printout_judge_name },
	{ "F_COURT_AREA",		generic_court_area },
	// 2017:244 long names for compatibility with printsub
	{ "F_REGARD_ADDR_SKIP_UCLP",	printout_regard_addr_skip_uclp },
	{ "F_REGARD_ADDR_UCLP_ONLY",	printout_regard_addr_uclp_only },
	{ "F_COURT_ADDR_SKIP_UCLP",	generic_court_addr_skip_uclp },
	{ NULL,			NULL }
};

char outreg_subpoena_kind(const TOutReg *outReg)
{
	switch (outReg->receiverType)
	{
		case UCN_INSTANCE_HIGHER : return SUBPOENA_KIND_SEND_APPEAL;
		case UCN_INSTANCE_LOWER : return SUBPOENA_KIND_RETURN_LOWER;
		case UCN_INSTANCE_EQUAL :
		case UCN_INSTANCE_COMPETENT : return SUBPOENA_KIND_SEND_JURISD;
		case UCN_INSTANCE_CONFLICT : return SUBPOENA_KIND_SEND_CONFLICT;
		case UCN_EXECUTOR_UCN :
		case UCN_EXECUTOR_CODE : return SUBPOENA_KIND_SEND_EXECUTOR;
		case UCN_INSTANCE_PROSEC : return SUBPOENA_KIND_SEND_PROSEC;
		case UCN_INSTANCE_RESERVED : return SUBPOENA_KIND_SEND_FALLBACK;
		default : fatal("%d: receiver type not supported by outreg_subpoena_kind", outReg->receiverType);
	}

	return '\0';
}

static print_outreg(TWindow *parent, TPrintOutParams &p, bool save, char subpoenaKind, TAddress *lReceiverAddress)
{
	TSubpoena subpoena;
	char target[SIZE_OF_PATH];
	TOffice office;
	bool filter = exist(Electrics[TYPE_TEXT].docketPath, false);
	bool result = false;

	subpoena.subpoenaKind = subpoenaKind;
	SubpoenaKind->Seek(subpoena.subpoenaKind);
	electric_name(p.outReg, target, TYPE_TEXT);

	if (office.Open(SubpoenaKind->htemplate, OFFICE_COPY_ALL, save && filter ? basename(target) : NULL))
	{
		TWaitWindow wait(parent, WAIT_MORE);
		TPrintOutReg printout(p, subpoenaKind, lReceiverAddress);

		if (COURT_CODE == COURT_BUROS && p.inRegEvents)
		{
			constant tr1start = office.Find("tr1 start");
			constant tr1break = office.Find("tr1 break");
			constant tr1final = office.Find("tr1 final");

			office.Emit(NULL, tr1start, printout_fields, &printout);

			while (p.inRegEvents->Count())
			{
				char kind = (*p.inRegEvents)[0]->kind;
				printout.bCount = 0;

				for (int i = 0; i < p.inRegEvents->Count(); i++)
					if ((*p.inRegEvents)[i]->kind == kind)
						printout.bCount++;

				printout.bInReg << (*p.inRegEvents)[0];
				office.Emit(tr1start, tr1break, printout_fields, &printout);

				for (int i = 0; i < p.inRegEvents->Count(); )
				{
					if ((*p.inRegEvents)[i]->kind == kind)
					{
						printout.bInReg << (*p.inRegEvents)[i];
						office.Emit(tr1break, tr1final, printout_fields, &printout);
						printout.bCount++;
						p.inRegEvents->Remove(i);
					}
					else
						i++;
				}
			}

			office.Emit(tr1final, NULL, printout_fields, &printout);
		}
		else
			office.Emit(NULL, NULL, printout_fields, &printout);

		if (save)
		{
			// 2013:163 FIX: block only if text path exists
			office.ShowSync(&wait, filter, "Изходящия документ ще бъде отворен", "Изходящия документ е записан");

			if (p.outReg->receiverType != UCN_INSTANCE_RESERVED)
			{
				result = true;
				subpoena.key = p.outReg->key;
				subpoena.kind = p.outReg->kind;
				subpoena.date = p.outReg->date;
				subpoena.ucnType = p.outReg->receiverType;
				strcpy(subpoena.ucn, p.outReg->receiver);
				subpoena.involvement = INVOLVEMENT_RECEIVER;
				subpoena.eventStatus = p.outReg->Status();
				subpoena.subpoenaNo = 1;
				subpoena.subpoenaDate = Today;
				subpoena.officiallyNotified = SUBPOENA_OFFICIALLY_NONNOTIFIED;	// :-)
				subpoena.receivedStatus = SRS_NONE;
				strcpy(subpoena.userUcn, Default->ucn);

				TSubpoena tSubpoena;
				tSubpoena << subpoena;

				if (tSubpoena.Try())
					subpoena.Update();
				else
					subpoena.Insert();
			}
			else	// 2015:175; 2017:157 replaced << w/ assignment
			{
				TSideWind sideWind;

				sideWind.key = p.outReg->key;
				sideWind.kind = p.outReg->kind;
				sideWind.date = p.outReg->date;
				sideWind.ucnType = p.lReceiver->ucnType;
				strcpy(sideWind.ucn, p.lReceiver->ucn);
				sideWind.involvement = INVOLVEMENT_RECEIVER;
				result = sideWind.Try() || sideWind.Insert();
			}

			if (filter)
				electric_copy(office.Name, target);

			wait.ResetForeground();
		}
		else
		{
			office.Show(&wait);
			result = true;
		}
	}

	return result;
}

static bool print_outreg(TWindow *parent, TPrintOutParams &p, bool save, char subpoenaKind, bool hasReceiver)
{
	TAnyAddress *lReceiverAddress = NULL;

	if (hasReceiver || (lReceiverAddress = get_address(parent, p.lReceiver, ADDRESS_BULGARIAN_ONLY)) != NULL)
	{
		TVirtPtr VP(lReceiverAddress);
		return print_outreg(parent, p, save, subpoenaKind, (TAddress *) lReceiverAddress);
	}

	return false;
}

bool print_outreg(TWindow *parent, const TOutReg *outReg, TInReg *inReg, TRCDKeyContainer *event, bool save,
	TGArray<TInRegEvent> *inRegEvents, const TConnect *lower)
{
	TTransGroup lReceiver;
	bool result = false;
	bool hasReceiver = outReg->receiverType != UCN_INSTANCE_RESERVED;
	TConnect tLower;
	TPrintOutParams p = { outReg, inReg, event, inRegEvents, lower ? lower : &tLower, &lReceiver };

	if (hasReceiver || get_ucn(parent, &lReceiver, UCN_REAL_SIDES, -1))
	{
		result = print_outreg(parent, p, save, outreg_subpoena_kind(outReg), hasReceiver);

		if (result && save && outReg->deliveryNotice)
			print_outreg(parent, p, false, SUBPOENA_KIND_DELIVERY_NOTICE, hasReceiver);
	}

	return result;
}

// ----- outreg_send_appeal --------------------------------------------------
static void print_subpoenas(mstr &m, const char *criteria)
{
	TSubpoena subpoena;
	TQuery q(criteria);

	while (q.Read())
	{
		subpoena << q;
		m.sep("\v");

		if (strchr(UCN_CITIZENS, subpoena.ucnType))
		{
			TCitizen citizen;

			strcpy(citizen.ucn, subpoena.ucn);
			citizen.ucnType = subpoena.ucnType;

			if (citizen.Get())
			{
				m.printf("%c.", *citizen.name);

				if (any(citizen.reName))
					m.printf("%c.", *citizen.reName);

				m.cat(citizen.family);

				if (any(citizen.reFamily))
					m.printf("-%s", citizen.reFamily);
			}
		}
		else
		{
			TSideWind sideWind;

			sideWind << subpoena;
			sideWind.PrintNames(m);
		}

		m.printf(" %s", Involvement->Seek(subpoena.involvement));

		if (subpoena.subpoenaReceived.Empty())
			m.printf(" дата на съобщението %D", &subpoena.subpoenaDate);
		else
		{
			m.printf(" изготвена на %D ", &subpoena.subpoenaDate);
			m.printf(subpoena.receivedStatus == SRS_NOT_RECEIVED ? "върната в цялост на %D" : " връчена на %D",
				&subpoena.subpoenaReceived);
		}
	}
}

static void print_surrounds(mstr &m, const char *criteria)
{
	TSurround surround;
	TSideWind sideWind;
	TQuery q(criteria);

	while (q.Read())
	{
		TGArray<TSideWind> sideWinds;
		msql w(sideWind.Name);

		surround << q;
		m.sep("\v\v");
		m.printf("%s вх.№ %ld/%ld", any(surround.text) ? surround.text : Kind->Seek(surround.kind), surround.key.no,
			surround.key.year);

		w.Add(&surround, "F_NO, F_YEAR");
		w.AddChar("F_TYPE", TYPE_SURROUND);
		w.AddChar("F_STATE", STATE_SURROUND);
		sideWind.LoadArray(&sideWinds, str(w));

		for (int i = 0; i < sideWinds.Count(); i++)
			print_links_names(m, sideWinds[i], true);
	}
}

static void print_inreg_judge(mstr &m, const TInReg *inReg, bool initials)
{
	char judge[SIZE_OF_UCN];
	*judge = '\0';

	if (strchr(KIND_LAWSUITS, inReg->lawsuit.type))
	{
		TLawsuit lawsuit;
		lawsuit.key = inReg->lawsuit;

		if (lawsuit.Get("F_JUDGE"))
			strcpy(judge, lawsuit.judge);
	}
	else
		strcpy(judge, inReg->judge);

	if (atob(judge))
	{
		if (initials)
			print_initials(m, judge, UCN_CITIZEN_UCN, true);
		else
			print_user(m, judge);
	}
}

#define inReg OFFICE_GROUP_CAST(TInReg)

static void inreg_origin(mstr &m, TGroup *group)
{
	Type->Seek(inReg->lawsuit.type);
	m.printf("%s %ld/%ld", Type->alias, inReg->lawsuit.no, inReg->lawsuit.year);
}

static void inreg_judge(mstr &m, TGroup *group) { print_inreg_judge(m, inReg, false); }
static void inreg_connects(mstr &m, TGroup *group) { print_connects(m, inReg->lawsuit, true, true, "с-в"); }

static void inreg_inreg(mstr &m, TGroup *group)
{
	m.printf("%s вх.№ %ld/%ld", Kind->Seek(inReg->kind), inReg->key.no, inReg->key.year);
}

static void inreg_decision_received(mstr &m, TGroup *group)
{
	TBaseWind subpoena("T_SUBPOENA", NULL);
	TDecision decision;
	msql w;

	subpoena.key = decision.key = inReg->lawsuit;
	subpoena.kind = decision.kind = inReg->decisionKind;
	subpoena.date = decision.date = inReg->decisionDate;

	if (strchr(KIND_ENDOCS, decision.kind) && !decision.Try("F_NO"))
	{
		if (decision.kind == KIND_SETTLEMENT)
			subpoena.kind = decision.kind = inReg->sessionKind;
		else
			say_no_data(decision.Name);
	}
	else if (decision.key.type == TYPE_INREG)
		subpoena.kind = decision.kind = *KIND_INREG_EVENTS;
	else if (decision.key.type == TYPE_REQUEST)
		subpoena.key = decision.key = inReg->lawsuit;

	subpoena.FloatCriteria(w);
	w.AddChars("F_SUBPOENA_KIND", SUBPOENA_KIND_ACT_COPIES);
	print_subpoenas(m, str(w));
}

static void inreg_appeal_received(mstr &m, TGroup *group)
{
	msql w("T_SUBPOENA");

	w.Add(group, "F_NO, F_YEAR, F_TYPE");
	w.AddChars("F_KIND", KIND_INREG_EVENTS);
	w.AddChar("F_EVENT_STATUS", RESULT_INSEND);	// from OutRegSendAppeal
	print_subpoenas(m, str(w));
}

static void inreg_appeal_response(mstr &m, TGroup *group)
{
	TSurround surround;
	msql w;

	surround.lawsuit = inReg->key;
	surround.FloatCriteria(w, "F_NO, F_YEAR, F_TEXT, F_KIND");
	print_surrounds(m, str(w));
}

static void inreg_legal_advice(mstr &m, TGroup *group)
{
	TSurround surround;
	msql w(surround.Name, "F_NO, F_YEAR, F_KIND");

	surround.lawsuit = inReg->lawsuit;
	w.Add(&surround, "F_LAWSUIT_NO, F_LAWSUIT_YEAR, F_LAWSUIT_TYPE");
	w.AddChar("F_KIND", KIND_HELP_SURROUND);
	print_surrounds(m, str(w));
}

static void inreg_judge_initials(mstr &m, TGroup *group) { print_inreg_judge(m, inReg, true); }

#undef inReg

static const offield inreg_fields[] =
{
	{ "F_ORIGIN",		inreg_origin },
	{ "F_JUDGE",		inreg_judge },
	{ "F_CONNECT",		inreg_connects },
	{ "F_INREG",		inreg_inreg },
	{ "F_DECISION_RECEIVED",inreg_decision_received },
	{ "F_APPEAL_RECEIVED",	inreg_appeal_received },
	{ "F_APPEAL_RESPONSE",	inreg_appeal_response },
	{ "F_LEGAL_ADVICE",	inreg_legal_advice },
	{ "F_JUDGE_INITIALS",	inreg_judge_initials },
	{ "F_USER_INITIALS",	generic_user_initials },
	{ NULL, NULL }
};

// 2016:236 support "save" - copied from print_outreg()
void outreg_send_appeal(TWindow *parent, const TOutReg *outReg, const TGArray<TInRegEvent> *inRegEvents, bool save)
{
	char target[SIZE_OF_PATH];
	TOffice office;
	bool filter = exist(Electrics[TYPE_TEXT].docketPath, false);

	electric_name(outReg, target, TYPE_TEXT, EXTRA_NO_OUTREG_SUP_APP);

	if (office.Open("OutRegL11.htm", OFFICE_COPY_ALL, save && filter ? basename(target) : NULL))
	{
		TWaitWindow wait(parent, WAIT_MORE);
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, null_fields, NULL);

		for (int i = 0; i < inRegEvents->Count(); i++)
		{
			TInReg inReg;

			if (i)
				office.EmitBreak();

			inReg << (*inRegEvents)[i];

			if (inReg.Get())
				office.Emit(tr1start, tr1final, inreg_fields, &inReg);
		}

		office.Emit(tr1final, NULL, null_fields, NULL);

		if (save)
		{
			office.ShowSync(&wait, filter, "Изходящия документ ще бъде отворен", "Изходящия документ е записан");

			if (filter)
				electric_copy(office.Name, target);
		}
		else
			office.Show(&wait);
	}
}
