#include "cac.h"

static bool copy_ucn_group(THTMLFile &h, TNamedGroup *source, TNamedGroup *target, TUCNGroup &group, TRCDKey *key,
	const CDate *date)
{
	msql m(group.Name);
	bool result = true;

	group << source;
	m.Add(&group, "F_UCN, F_UCN_TYPE");

	TQuery q(str(m));
	while (q.Read())
	{
		const char *wrong = NULL;

		group << q;
		group.ucnType = target->ucnType;
		strcpy(group.ucn, target->ucn);

		if (group.Try())
			wrong = "дублиране на данни";
		else if (!group.Insert())
			wrong = "грешка при запис";

		if (wrong)
		{
			if (key)
				h.printf("%s %ld/%ld", Type->Seek(key->type), key->no, key->year);

			h.printf(" %s" + !key, group.Name);

			if (date)
				h.printf(" от %D", date);

			h.printf(": %s\n", wrong);
			// 2014:274 FIX: don't return of error, most copies are unchecked
		}
	}

	return result;
}

static inline bool copy_ucn_group(THTMLFile &h, TNamedGroup *source, TNamedGroup *target, TUCNGroup &ucnGroup)
{
	return copy_ucn_group(h, source, target, ucnGroup, NULL, NULL);
}

static inline void copy_ucn_group(THTMLFile &h, TNamedGroup *source, TNamedGroup *target, TBaseWind &baseWind)
{
	copy_ucn_group(h, source, target, baseWind, &baseWind.key, &baseWind.date);
}

static void clean_ucn_group(THTMLFile &h, TNamedGroup *source, TUCNGroup &group)
{
	msql m(group.Name, MQ_DELETE);

	group << source;
	m.Add(&group, "F_UCN, F_UCN_TYPE");

	if (!group.DataBase->Execute(str(m), ET_ERROR))
		h.printf("%s: грешка при изтриване\n", str(m));
}

// 2013:294 LPR: +related, safer
static void copy_and_clean(THTMLFile &h, TNamedGroup *source, TNamedGroup *target, TUCNGroup &ucnGroup, bool move)
{
	if (!move || copy_ucn_group(h, source, target, ucnGroup))
		clean_ucn_group(h, source, ucnGroup);
}

// 2004:015 LPR: type support added for TLinkWind and TSubpoena
// 2004:041 LPR: key support ported from 1.1820+17
// key support is only required by TLawsuitDistribution
static void alter_any_group(TNamedGroup *source, TNamedGroup *target, TGroup &group, const char *name, char *data,
	const char *typeName = NULL, char *typeData = NULL, bool key = false)
{
	msql m(group.Name);

	strcpy(data, source->ucn);
	m.AddString(name, data);
	if (typeName)
		m.AddChar(typeName, source->ucnType);

	TQuery q(str(m));
	while (q.Read())
	{
		m.cpy(name);
		group << q;
		strcpy(data, target->ucn);

		if (typeData)
		{
			*typeData = target->ucnType;
			m.printf(", %s", typeName);
		}

		if (!key)
			group.Update(str(m));
		else
		{
			group.Insert();
			strcpy(data, source->ucn);
			if (typeData != NULL)
				*typeData = source->ucnType;
			group.Delete(true);
		}
	}
}

// ----- renumber ------------------------------------------------------------
static bool renumber(TWindow *parent, TNamedGroup *source, TNamedGroup *target, bool move)
{
	char uxnType = ucn2uxn(target->ucnType);

	// First of all, several checks
	if (!strchr(UXN_REJOINABLES, uxnType))
		fatal("%d: uxnType not supported by %s", uxnType, move ? "renumber" : "join");
	//if (ucn2uxn(source->ucnType) != ucn2uxn(target->ucnType))
	//	fatal("%d: can not be renumbered to %d", source->ucnType, target->ucnType);
	//if (move && is_code(target->ucnType))
	//	fatal("%d: renumber target can't be code", target->ucnType);

	// 2nd, several more checks
	if (source->ucnType == UCN_CITIZEN_UCN)
	{
		TUser user;
		user << source;

		if (user.Try())
			return error("Потребителите са забранени за модификация.");

		TCitizen citizen;
		citizen << source;

		if (citizen.Try() && is_internal(citizen.ucn))
			return error("Лицето е забранено за модификация.");

		if (is_real_ucn(source) && eportal_guid(source, INVOLVEMENT_EPORT_SIDE))
			return error("Лицето е регистрирано в %s с това ЕГН.", EPORTAL_NAME);

		if (target->ucnType == UCN_CITIZEN_UCN && is_real_ucn(source) && is_real_ucn(target) &&
			eportal_guid(source, INVOLVEMENT_LAWER) && CDate(source->ucn) != CDate(target->ucn))
		{
			return error("Лицето е регистрирано в %s като адвокат, но датите на раждане в единните граждански "
				"номера не съвпадат.", EPORTAL_NAME);
		}
	}

	if (source->ucnType == UCN_CITIZEN_CODE && target->ucnType == UCN_CITIZEN_UCN && is_real_ucn(target) &&
		eportal_guid(source, INVOLVEMENT_LAWER))
	{
		TKnownPerson person;

		person.ucnType = source->ucnType;
		strcpy(person.ucn, source->ucn);
		person.involvement = INVOLVEMENT_LEGALESE;

		if (person.Try("F_BIRTH_DATE") && !person.birthDate.Empty() && person.birthDate != CDate(target->ucn))
		{
			return error("Лицето е регистрирано в %s като адвокат, но датата му на раждане не съвпада с "
				"указания единен граждански номер.", EPORTAL_NAME);
		}
	}

	if (citizen_in_progress(source))
		return error("В момента се изпращат или изтриват данни за лицето в %s.", EPORTAL_NAME);

	if (move == target->Try())
		return error("Лице с %s %s %s съществува.", UCNType->Seek(target->ucnType), target->ucn, move ? "вече" : "не");

	mstr m;  // 2007:186 +related: verbose log; 2018-11-23: LPR: was s[...]
	// 2011:152 TRQ: sep[arate prints to avoid double ucntype seek
	m.printf("%s %s %c>", UCNType->Seek(source->ucnType), source->ucn, move ? '-' : '=');
	m.printf(" %s %s", UCNType->Seek(target->ucnType), target->ucn);
	log(move ? LOG_RENUMBER : LOG_JOIN, move ? CM_ADMIN_RENUMBER_UXN : CM_ADMIN_JOIN_UXN, str(m));

	// What we will need - starring in the order of appearance
	TFineComposition composition;	// UCN only, many records, many fields

	// 2004:012 BRW: using TSideWind and TLinkWind
	TSideWind sideWind;			// key, many records
	TLinkWind linkWind;			// key, many records
	TSubpoena subpoena;			// key, many records
	TLawsuit lawsuit;				// UCN only, many records, many fields
	TSession session;				// UCN only, many records, many fields
	TCitizen citizen;				// key, physical only, one record
	TFirm firm;					// key, judicial only, one record
	TDamn damn;					// key, damn only, one record
	TAddress address;				// key, one record
	TForeignAddress foreignAddress;	// key, one record
	TJudgement judgement;			// key, many records
	TAccusation accusation;			// key, many records
	TOldCitizen oldCitizen;			// key, physical only, one record
	TKnownPerson known;			// key, physical only, one record
	// 2004:015 BUGFIX: handle any post-2K:326 fields
	TExpense expense;				// key, many records
	TRegisteredMail registeredMail;	// key, many records
	TRequestResolution resolution;	// UCN only, many records, one field
	TSurround surround;			// UCN only, many records, one field
	TInReg inReg;				// UCN only, many records, one field
	TInRegEvent inRegEvent;			// UCN only, many records, one field
	TSched sched;				// UCN only, many records, one field
	TBankrupt bankrupt;			// key, judicial only, one record
	TOldFirm oldFirm;				// key, judicial only, one record
	TPayment payment;				// physical only, many records, many fields
	TExecListPerson person;			// side key, many records, one set
	TExecListCollect collect;		// many records, one field

	// 2010:278 IRQ: logging
	THTMLFile h;
	h.OpenTemp();
	TWaitWindow wait(parent, WAIT);

	// 2011:082 TRQ: double-seek -> split
	h.printf("%s на %s %s ", move ? "Преномериране" : "Присъединяване", UCNType->Seek(source->ucnType), source->ucn);
	h.printf("към %s %s\n", UCNType->Seek(target->ucnType), target->ucn);

	// copy various TBaseWinds and TUCNGroups
	copy_ucn_group(h, source, target, sideWind);
	copy_ucn_group(h, source, target, linkWind);
	copy_ucn_group(h, source, target, subpoena);
	copy_ucn_group(h, source, target, judgement);
	copy_ucn_group(h, source, target, accusation, &accusation.key, NULL);
	copy_ucn_group(h, source, target, expense, &expense.key, &expense.date);
	copy_ucn_group(h, source, target, registeredMail, &registeredMail.doc, &registeredMail.date);
	copy_ucn_group(h, source, target, known);
	copy_ucn_group(h, source, target, bankrupt);

	if (source->ucnType == UCN_CITIZEN_UCN)
	{
		// alter T_COMPOSITION
		alter_any_group(source, target, composition, "F_JUDGE", composition.judge);
		alter_any_group(source, target, composition, "F_SECRETAR", composition.secretar);
		// 2005:102 LPR: comout obsolete fields; 2005:318 removed them
		alter_any_group(source, target, composition, "F_JUDGE_X", composition.judgeX);
		alter_any_group(source, target, composition, "F_REJUDGE_X", composition.reJudgeX);
		alter_any_group(source, target, composition, "F_PRESIDENT", composition.president);
		// alter T_LAWSUIT; 2004:015 LPR: BUGFIX: F_REJUDGE
		alter_any_group(source, target, lawsuit, "F_JUDGE", lawsuit.judge);
		alter_any_group(source, target, lawsuit, "F_REJUDGE", lawsuit.rejudge);
		// alter T_SESSION
		alter_any_group(source, target, session, "F_JUDGE", session.judge);
		alter_any_group(source, target, session, "F_SECRETAR", session.secretar);
		alter_any_group(source, target, session, "F_PROSECUTOR", session.prosecutor);
		alter_any_group(source, target, session, "F_JUROR", session.juror);
		alter_any_group(source, target, session, "F_REJUROR", session.reJuror);
		alter_any_group(source, target, session, "F_RESERVE", session.reserve);
		alter_any_group(source, target, session, "F_JUDGE_X", session.judgeX);
		alter_any_group(source, target, session, "F_REJUDGE_X", session.reJudgeX);
		alter_any_group(source, target, session, "F_JUROR_X", session.jurorX);
		alter_any_group(source, target, session, "F_PRESIDENT", session.president);
		// 2004:015 LPR: BUGFIX: handle any post-2K:326 fields
		alter_any_group(source, target, resolution, "F_JUDGE", resolution.judge);
		alter_any_group(source, target, surround, "F_JUDGE", surround.judge);
		alter_any_group(source, target, inReg, "F_JUDGE", inReg.judge);
		alter_any_group(source, target, inRegEvent, "F_JUDGE", inRegEvent.judge);
		alter_any_group(source, target, sched, "F_JUDGE", sched.judge);
		// 2009:174 moved here from CITIZEN || FIRM below, removed usedUcnType
		alter_any_group(source, target, subpoena, "F_USER_UCN", subpoena.userUcn);
		// 2010:006 moved here from CITIZEN || FIRM below, removed delivererUcnType
		alter_any_group(source, target, subpoena, "F_DELIVERER_UCN", subpoena.delivererUcn);
		alter_any_group(source, target, person, "F_UCN", person.ucn, "F_UCN_TYPE", &person.ucnType, true);
		alter_any_group(source, target, collect, "F_UCN", collect.ucn, "F_UCN_TYPE", &collect.ucnType);

		// 2010:116 LPR/TRQ: renumber juror distrib
		TJurorDistrib distrib;
		msql m(distrib.Name);

		m.AddString("F_JUROR", source->ucn);
		m.NextOr();
		m.AddString("F_REJUROR", source->ucn);
		m.NextOr();
		m.AddString("F_JUROR_X", source->ucn);
		m.NextOr();
		m.AddString("F_RESERVE", source->ucn);
		m.NextOr();
		m.AddLike("F_MOTIVES", source->ucn, true);

		bool dirty = false;
		TQuery q(str(m));

		while (q.Read())
		{
			distrib << q;

			for (int i = 0; i < JUROR_COUNT; i++)
			{
				if (!strcmp(distrib.jurors[i], source->ucn))
				{
					strcpy(distrib.jurors[i], target->ucn);
					dirty = true;
				}
			}

			// string 10:10 replacement only
			char *entry = ncstr(distrib.motives);

			for (char *ts; (ts = strchr(entry, '\n')) != NULL; entry = ts + 1)
			{
				if (!strncmp(entry + 1, source->ucn, LENGTH_OF_UCN))
				{
					memcpy(entry + 1, target->ucn, LENGTH_OF_UCN);
					dirty = true;
				}
			}

			if (dirty)
				distrib.Update();
		}
	}

	if (uxnType == UXN_CITIZEN || uxnType == UXN_FIRM)
	{
		// alter TLinkWind
		alter_any_group(source, target, linkWind, "F_PROXY_UCN", linkWind.proxyUcn, "F_PROXY_UCN_TYPE", &linkWind.proxyUcnType, true);
		alter_any_group(source, target, linkWind, "F_REDIRECT_UCN", linkWind.redirectUcn, "F_REDIRECT_UCN_TYPE", &linkWind.redirectUcnType);
		// alter T_SUBPOENA
		alter_any_group(source, target, subpoena, "F_PROXY_UCN", subpoena.proxyUcn, "F_PROXY_UCN_TYPE", &subpoena.proxyUcnType);
		alter_any_group(source, target, subpoena, "F_REDIRECT_UCN", subpoena.redirectUcn, "F_REDIRECT_UCN_TYPE", &subpoena.redirectUcnType);
		alter_any_group(source, target, subpoena, "F_LRECEIVER_UCN", subpoena.lReceiverUcn, "F_LRECEIVER_UCN_TYPE", &subpoena.lReceiverUcnType);
		// alter T_PAYMENT
		alter_any_group(source, target, payment, "F_UCN", payment.ucn, "F_UCN_TYPE", &payment.ucnType);
		alter_any_group(source, target, payment, "F_OBL", payment.obl, "F_OBL_TYPE", &payment.oblType);
		// alter T_EPORT_GUID
		eportal_renumber_guid(source, target);	// physical only, many records, one field
	}

	if (move)
	{
		// copy T_ADDRESS, T_FOREIGN_ADDRESS
		copy_ucn_group(h, source, target, address);
		copy_ucn_group(h, source, target, foreignAddress);
	}

	// clean obsoleted data - after move addresses
	clean_ucn_group(h, source, sideWind);
	clean_ucn_group(h, source, linkWind);
	clean_ucn_group(h, source, subpoena);
	clean_ucn_group(h, source, judgement);
	clean_ucn_group(h, source, accusation);
	clean_ucn_group(h, source, expense);
	clean_ucn_group(h, source, registeredMail);
	clean_ucn_group(h, source, address);
	clean_ucn_group(h, source, foreignAddress);
	clean_ucn_group(h, source, bankrupt);
	clean_ucn_group(h, source, known);

	// copy and clean T_CITIZEN/T_FIRM - this code MUST be last
	switch (uxnType)
	{
		case UXN_CITIZEN :
		{
			// 2010:278 LPR: reverse order of importance
			copy_and_clean(h, source, target, oldCitizen, move);
			copy_and_clean(h, source, target, citizen, move);
			break;
		}
		case UXN_FIRM :
		{
			copy_and_clean(h, source, target, oldFirm, move);
			copy_and_clean(h, source, target, firm, move);
			break;
		}
		// 2013:295 N.B. obsolete
		case UXN_DAMN :
		{
			copy_and_clean(h, source, target, damn, move);
			break;
		}
	}

	h.Show(&wait, false);
	return true;
}

static bool renumber(TWindow *parent, TUCNGroup *sourceGroup, TUCNGroup *targetGroup, bool move)
{
	TNamedGroup *source = TNamedGroup::NewGroup(sourceGroup->ucnType);
	TNamedGroup *target = TNamedGroup::NewGroup(targetGroup->ucnType);
	bool result;

	*source << sourceGroup;
	*target << targetGroup;
	result = renumber(parent, source, target, move);

	delete source;
	delete target;
	return result;
}

bool renumber(TWindow *parent, TUCNGroup *source, TUCNGroup *target)
{
	return renumber(parent, source, target, true);
}

bool join(TWindow *parent, TUCNGroup *source, TUCNGroup *target)
{
	return renumber(parent, source, target, false);
}

bool recompose(TLawsuit *source, TLawsuit *target)
{
	if (target->composition != source->composition && !target->Update("F_COMPOSITION"))
		return false;

	TSession session;
	session << target;

	msql m;
	session.FloatCriteria(m);

	TQuery q(str(m));
	while (q.Read())
	{
		session << q;

		if (session.composition != target->composition)
		{
			session.composition = target->composition;
			session.Update("F_COMPOSITION");
		}
	}

	return true;
}

// 2004:015 BRW: new low-level street renumberer, key support
// 2004:022 LPR: confirmed by LRQ: bool -> void, no "return"
static void alter_street_group(TStreet *source, TStreet *target, TGroup *group, long *data, bool key)
{
	msql m(group->Name);

	m.AddLong("F_UCLP", source->uclp);
	m.AddLong("F_STREET", source->street);
	TQuery q(str(m));

	while (q.Read())
	{
		*group << q;
		*data = target->street;

		if (!key)
			group->Update("F_STREET");
		else
		{
			group->Insert();
			*data = source->street;
			group->Delete(true);
		}
	}
}

bool join(TStreet *source, TStreet *target)
{
	TAddress address;
	TStreetMap streetMap;
	TRegisteredMail mail;
	TSubpoena subpoena;
	TAnnounce announce;

	alter_street_group(source, target, &address, &address.street, false);
	alter_street_group(source, target, &streetMap, &streetMap.street, true);
	alter_street_group(source, target, &mail, &mail.street, false);
	alter_street_group(source, target, &subpoena, &subpoena.street, false);
	alter_street_group(source, target, &announce, &announce.street, false);

	return source->Delete(true);
}

bool renumber(TStreet *source, TStreet *target)
{
	target->uclp = source->uclp;
	strcpy(target->name, source->name);
	return target->Insert() ? join(source, target) : false;
}

TRenumberContainer::TRenumberContainer(TRCDKeyContainer *source, TRCDKeyContainer *tTarget)
	: target(tTarget)
{
	*this << source;
	Signal(ELECTRIC_RENUMBER);
}

static void log_renum(TRCDKeyContainer *source, TRCDKeyContainer *target)
{
	char s[SIZE_OF_LOG_TEXT];
	sprintf(s, "-> %ld/%ldг.", target->key.no, target->key.year);
	log(LOG_RENUMBER, PrimaryCommand, source, s);
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void renumber_eispps(TOriginContainer *source, TOriginContainer *target)
{
	for (constant *name = EISPP_TABLE_NAMES; *name; name++)
	{
		msql m(*name, MQ_UPDATE);

		m.printf("F_NO = %ld, F_YEAR = %ld", target->key.no, target->key.year);
		m.Add(source, "F_NO, F_YEAR, F_TYPE");
		source->DataBase->Execute(str(m), ET_ERROR);
	}
}
#endif  // APPEAL || TYPE == AREAL || TYPE == MILITARY || REGIONAL

#if COURT_TYPE == COURT_ADMIN
void renumber_eispps(TOriginContainer *, TOriginContainer *)
{
}
#endif  // ADMIN

// 2017:151 outline from renumber_doc()
static void redate_interval(TOriginContainer *source, TOriginContainer *target)
{
	TLawInterval interval(source->key, source->date);

	if (interval.Get("F_NO"))
	{
		interval.Delete(true);
		interval << target;
		interval.flags = 0;	// 2009:282 FIX: was missing
		interval.Insert();
	}
}

static void renumber_doc(TOriginContainer *source, TOriginContainer *target, TLawsuit *lawsuit)
{
	// 2004:355 common index support, check for errors
	// Sides are xfered automatically; 2002:035 Or walns
	// 2003:332 BRW: Insert() -> Combine()
	if (!target->Combine(source))
		return;

	TRenumberContainer(source, target);
	TGArray<TSideWind> sideWinds;

	if (lawsuit)
	{
		xfer_bank_case(lawsuit, NULL, NULL, XFER_ADD);
		eportal_send(NULL, lawsuit, LOG_WRITE);
		load_sidewinds(lawsuit, &sideWinds);
		eportal_send(NULL, &sideWinds, lawsuit, LOG_WRITE);
		sideWinds.Flush();
	}

	log_renum(source, target);
	delete_winds(source, &sideWinds);

	if (source->Delete(true))
	{
		if (lawsuit)
		{
			TLawsuit *tLawsuit = (TLawsuit *) source;

			xfer_bank_case(tLawsuit, &sideWinds, NULL, XFER_DELETE);
			eportal_send(NULL, &sideWinds, tLawsuit, LOG_DELETE);
			eportal_send(NULL, tLawsuit, LOG_DELETE);
		}
	}
	else
		error("Стария документ НЕ е изтрит.");

	if (source->key.type == TYPE_FIRM_LAW)
	{
		TDamn damn;

		// 2004:167 LPR: conditional
		if (find_damn((TLawsuit *) source, &damn))
		{
			damn.key = target->key;

			if (!damn.Update("F_NO, F_YEAR, F_TYPE"))
				error("Свързаната с делото фирма НЕ се записа.");
		}
	}

	// 2008:010 Rewritten
	if (lawsuit && lawsuit->source.type == TYPE_REQUEST)
		divert_source(lawsuit, &source->date);

	// 2007:246 LRQ: FIX: was missing
	TGArray<TConnect> connects;

	// 2008:009 LPR: only if combined
	if (load_connects(source, &connects))
	{
		TConnect connect;

		write_connects(target, &connects, "новият документ/дело");
		connect.key = source->key;
		connect.Delete(false);
	}

	// 2009:223 FIX: was missing
	if (lawsuit)
		redate_interval(source, target);
}

void renumber_doc(TRequest *source, TRequest *target)
{
	*RequestIndex << target;

	if (RequestIndex->Insert())
	{
		*RequestIndex << source;
		RequestIndex->Delete(true);

		renumber_doc(source, target, NULL);

		if (any(source->eispp_nmr))
			renumber_eispps(source, target);
	}
}

void renumber_law(TLawsuit *source, TLawsuit *target)
{
	if (source->key.no != target->key.no || source->key.year != target->key.year)
	{
		renumber_doc(source, target, target);
		eispp_renumber(source, target);
	}
	else if (source->date != target->date || source->previousDate != target->previousDate ||
		source->receivedDate != target->receivedDate)
	{
		TLawsuit *lawsuit = target;

		if (lawsuit->Update("F_DATE, F_RETURNED_DATE, F_PREVIOUS_DATE") && source->date != target->date)
		{
			xfer_bank_case(source, NULL, NULL, XFER_DELETE);
			move_winds(source, target);
			xfer_bank_case(target, NULL, NULL, XFER_ADD);
			eportal_send(NULL, target, LOG_WRITE);
			redate_interval(source, target);		// 2017:151 IRQ: FIX: was missing

			if (lawsuit->source.type == TYPE_REQUEST)
				divert_source(lawsuit, &source->date);
		}
	}
}

static bool renumber_dtc(TWindow *parent, TDatedContainer *source, TDatedContainer *target, bool surround)
{
	if (!TRenumberDocDialog(parent, target).Execute() == IDOK)
		return false;

	sign eportalSendable = (source->key.type == TYPE_INREG) ? ((const TInReg *) source)->EPortalSendable() : false;

	// 2004:355 common index support, check for errors
	// 2007:200 init required only if inreg/surround check
	if (source->key.no != target->key.no || source->key.year != target->key.year)
	{
		// 2007:200 N.B. assuming target is inreg or surround
		*RequestIndex << target;
		if (!RequestIndex->Insert())
			return false;

		*RequestIndex << source;
		RequestIndex->Delete(true);

		// Sides are xfered automatically
		// 2003:332 BRW: Insert() -> Combine()
		if (!target->Combine(source))
			return false;

		TRenumberContainer(source, target);

		if (surround)
			xfer_bank_surround(NULL, (TSurround *) target, NULL, XFER_ADD);
		else
			xfer_bank_event(NULL, NULL, NULL, NULL, (TInReg *) target, NULL, NULL, XFER_ADD);

		log_renum(source, target);
		TGArray<TSideWind> sideWinds;

		delete_winds(source, &sideWinds);

		if (source->Delete(true))
		{
			if (surround)
				xfer_bank_surround(NULL, (TSurround *) source, &sideWinds, XFER_DELETE);
			else
				xfer_bank_event(NULL, NULL, NULL, NULL, (TInReg *) source, &sideWinds, NULL, XFER_DELETE);
		}
		else
			error("Стария документ НЕ е изтрит.");

		if (eportalSendable)
			eportal_send(NULL, (const TInReg *) source, NULL, LOG_DELETE, eportalSendable);
	}
	// 2011:175 FIX: copied from re# request
	// 2013:119 LPR: check date first, as in re# lawsuit
	else if (source->date != target->date && target->Update("F_DATE"))
	{
		if (surround)
			xfer_bank_surround(NULL, (TSurround *) source, NULL, XFER_DELETE);
		else
			xfer_bank_event(NULL, NULL, NULL, NULL, (TInReg *) source, NULL, NULL, XFER_DELETE);

		move_winds(source, target);

		if (surround)
			xfer_bank_surround(NULL, (TSurround *) target, NULL, XFER_ADD);
		else
			xfer_bank_event(NULL, NULL, NULL, NULL, (TInReg *) target, NULL, NULL, XFER_ADD);
	}

	if (eportalSendable)
		eportal_send(NULL, (const TInReg *) source, NULL, LOG_WRITE, eportalSendable);

	return true;
}

bool renumber_surround(TWindow *parent, TSurround *surround)
{
	TSurround tSurround;
	tSurround << surround;
	return renumber_dtc(parent, surround, &tSurround, true);
}

bool renumber_inreg(TWindow *parent, TInReg *inReg)
{
	TInReg tInReg;
	tInReg << inReg;
	return renumber_dtc(parent, inReg, &tInReg, false);
}
