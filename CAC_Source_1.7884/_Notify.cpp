#include "cac.h"

// ----- TNotifying --------------------------------------------------------
TNotifying::TNotifying(TWindow *parent, int tSideIndex, char evStatus, TSubpoena &tSubpoena, TRCDKeyContainer *keyContainer)
:
	pParent(parent), result(evStatus), sideIndex(tSideIndex), subpoena(tSubpoena),
	pKeyContainer(keyContainer), rSentSubpoena(NULL)
{
}

const char *TNotifying::Prefix()
{
	return "Уведомяване на";
}

bool TNotifying::InsertRegisteredMail()
{
	TRegisteredMail mail;

	mail.flags = subpoena.byPost & OUTREG_POSTAL;

	if ((mail.flags == OUTREG_R || mail.flags == OUTREG_AR || mail.flags == OUTREG_PACKAGE) && FillRegMailReceiver(mail))
	{
		mail.doc = subpoena.key;
		mail.date = Today;

		mail.lawsuitType = find_college(pKeyContainer);
		strcpy(mail.text, SubpoenaKind->alias);
		mail.flags = subpoena.byPost;
		mail.status = REGISTERED_MAIL_PREPARED;
		return mail.Insert();
	}

	return true;
}

bool TNotifying::FillRegMailReceiver(TRegisteredMail &mail)
{
	TSideWind s;

	if (!subpoena.GetAddressSide(s))
		return false;

	mail << subpoena;
	mail.ucnType = s.ucnType;
	strcpy(mail.ucn, s.ucn);
	return mail.uclp;
}

bool TNotifying::GetAddress(TUCNGroup *ucnGroup, bool foreign)
{
	int addrMode;

	switch (subpoena.subpoenaKind)
	{
		case SUBPOENA_KIND_NOTIFY_PHONE : addrMode = ADDRESS_REQUIRE_PHONE; break;
		case SUBPOENA_KIND_NOTIFY_EMAIL : addrMode = ADDRESS_REQUIRE_EMAIL; break;
		default : addrMode = foreign ? ADDRESS_ALLOW_FOREIGN : ADDRESS_BULGARIAN_ONLY;
	}

	return get_address(pParent, subpoena, ucnGroup, addrMode);
}

bool TNotifying::PreSetDeliverer()
{
	// 2018-01-16: FIX: regression: OUTREG_ELECTRON_X
	if (WRITINGS_ACTIVE && COURT_WRIT && subpoena.receivedStatus == SRS_FOR_DELIVER)
	{
		SubpoenaKind->Seek(subpoena.subpoenaKind);

		if (SubpoenaKind->genericKind)
		{
			TGArray<TUserAliasGroup> deliverers;
			int count = 0;

			UserAliasGroup->LoadData(&deliverers, C2S[POST_DELIVERER], FLAG_ACTIVE);

			for (int i = 0; i < deliverers.Count(); i++)
			{
				if (deliverers[i]->Serves(subpoena.uclp, subpoena.street, subpoena.adrNo, subpoena.blockNo))
				{
					if (++count == 2)
						break;

					strcpy(subpoena.delivererUcn, deliverers[i]->ucn);
				}
			}

			if (count != 1)
				*subpoena.delivererUcn = '\0';
		}
	}

	return true;
}

bool TNotifying::SpecificPreparations()
{
	return true;
}

bool TNotifying::PrepareForNotify(TLinkWind &wLink)
{
	SubpoenaKind->Seek(subpoena.subpoenaKind);

	if (SubpoenaKind->AddressRequired())
	{
		TSideWind sideWind;

		if (!wLink.GetAddressSide(sideWind) ||
			!GetAddress(&sideWind, any(SubpoenaKind->htemplate) || SubpoenaKind->proc == SK_PROC_NOTIFYING))
		{
			return false;
		}
	}

	subpoena << wLink;
	subpoena.sideNo = sideIndex + 1;
	subpoena.eventStatus = result;
	subpoena.officiallyNotified = SUBPOENA_OFFICIALLY_NONNOTIFIED;
	strcpy(subpoena.userUcn, Default->ucn);
	subpoena.receivedStatus = SubpoenaKind->InitialStatus();

	if (subpoena.receivedStatus == SRS_NULL)  // 2018-01-19
		subpoena.receivedStatus = subpoena.InitialStatus();

	return PreSetDeliverer() && SpecificPreparations();
}

bool TNotifying::AlternateInsert(const TSubpoenaVariantGroup *variant, char subpoenaKind)
{
	return variant->execList || strchr(SUBPOENA_KIND_LEGAL_AIDS, subpoenaKind);
}

bool TNotifying::Insert(TSubpoenaVariantGroup *variant, bool edit)
{
	return AlternateInsert(variant, subpoena.subpoenaKind) ? InsertAlternate(variant, edit) : InsertStandard(variant);
}

static void log_inserted(const TSubpoena &subpoena)
{
	if (subpoena.subpoenaDate != Today)
	{
		mstr m;

		SubpoenaKind->Seek(subpoena.subpoenaKind);
		m.printf("Призовка %s %0D", SubpoenaKind->alias, &subpoena.subpoenaDate);
		log_subpoena(LOG_PRINT, &subpoena, str(m));
	}
}

bool TNotifying::InsertStandard(TSubpoenaVariantGroup *variant)
{
	subpoena.multi = variant->links.Count() ? SUBPOENA_LEADER : SUBPOENA_SINGLE;

	if (!subpoena.Insert())
		return false;

	log_inserted(subpoena);
	variant->Affected(&subpoena);
	eportal_send(&subpoena, variant->lawsuit, LOG_WRITE, str(variant->receiver));
	InsertRegisteredMail();

	// 2008:176 LPR: store the additional subpoenas, if any...
	if (subpoena.multi == SUBPOENA_LEADER)
	{
		TSubpoena tSubpoena;

		tSubpoena << subpoena;
		tSubpoena.multi = SUBPOENA_MULTI;

		for (int i = 0; i < variant->links.Count(); i++)
		{
			tSubpoena << variant->links[i];
			tSubpoena.Insert();
			variant->Affected(&tSubpoena);
			eportal_send(&subpoena, variant->lawsuit, LOG_WRITE, str(variant->receiver));
		}
	}

	return true;
}

bool TNotifying::InsertAlternate(TSubpoenaVariantGroup *variant, bool edit)
{
	bool result = false;
	TGroupArray &array = variant->execList ? (TGroupArray &) variant->links : (TGroupArray &) variant->laids;

	TSubpoena tSubpoena;
	tSubpoena << subpoena;

	for (int i = 0; i < array.Count(); i++)
	{
		TBaseWind *baseWind = TYPECHECK_PTR_CAST(TBaseWind, array[i]);
		TTryAddress *tryAddress = dynamic_cast<TTryAddress *> (baseWind);

		tSubpoena << baseWind;

		if (tryAddress->address)
			tSubpoena << tryAddress->address;

		if (variant->execList)
		{
			tSubpoena << variant->execList;

			if (edit)
				tSubpoena.subpoenaDate = variant->execList->execListDate;
		}

		if (tSubpoena.Save(edit))
		{
			if (!result)
				log_inserted(tSubpoena);

			variant->Affected(&tSubpoena);
			result = true;

			if (i == 0)		// laid[0] = startup side
				subpoena << tSubpoena;
		}
	}

	return result;
}

bool TNotifying::Notify(TSubpoenaVariantGroup *variant)
{
	if (PrepareForNotify(variant->preLink))
	{
		bool edit = variant->execList;

		SubpoenaKind->Seek(subpoena.subpoenaKind);

		// TODO? check that printing required matches any(SubpoenaKind->htemplate)
		if (any(SubpoenaKind->htemplate) ? print_subpoena(this, SubpoenaKind->htemplate, variant, edit) :
			Insert(variant, edit))
		{
			return true;
		}
	}

	return false;
}

// ----- TNotifySubpoena -----------------------------------------------------
TNotifySubpoena::TNotifySubpoena(TWindow *parent, int sideIndex, char evStatus, TSubpoena &subpoena,
	TRCDKeyContainer *keyContainer)
:
	TNotifying(parent, sideIndex, evStatus, subpoena, keyContainer)
{
}

const char *TNotifySubpoena::Prefix()
{
	return pKeyContainer->Status() == RESULT_FIXED ? "Призовка за" : "Съобщение за";
}

// ----- TDecisionAnnounce ---------------------------------------------------
TDecisionAnnounce::TDecisionAnnounce(TWindow *parent, int sideIndex, char evStatus, TSubpoena &subpoena,
	TRCDKeyContainer *keyContainer)
:
	TNotifying(parent, sideIndex, evStatus, subpoena, keyContainer)
{
}

const char *TDecisionAnnounce::Prefix()
{
	return "СРГ за";
}

// ----- TResolutionAnnounce -----------------------------------------------
TResolutionAnnounce::TResolutionAnnounce(TWindow* parent, int sideIndex, char evStatus, TSubpoena &subpoena,
	TRCDKeyContainer *keyContainer)
:
	TNotifying(parent, sideIndex, evStatus, subpoena, keyContainer)
{
}

const char *TResolutionAnnounce::Prefix()
{
	return "Съобщение за";
}

// ----- TInRegEventAnnounce -------------------------------------------------
TInRegEventAnnounce::TInRegEventAnnounce(TWindow *parent, int sideIndex, char evStatus, TSubpoena &subpoena,
	TRCDKeyContainer *keyContainer)
:
	TNotifying(parent, sideIndex, evStatus, subpoena, keyContainer)
{
}

const char *TInRegEventAnnounce::Prefix()
{
	return "Съобщение за";
}

// ----- TLetterTo -----------------------------------------------------------
TLetterTo::TLetterTo(TWindow* parent, int sideIndex, char evStatus, TSubpoena &subpoena, TRCDKeyContainer *keyContainer,
	TSubpoena *tSentSubpoena)
:
	TNotifying(parent, sideIndex, evStatus, subpoena, keyContainer)
{
	rSentSubpoena = tSentSubpoena;
}

const char *TLetterTo::Prefix()
{
	return "Писмо относно";
}

bool TLetterTo::FillRegMailReceiver(TRegisteredMail &mail)
{
	mail << subpoena;
	mail.ucnType = subpoena.lReceiverUcnType;
	strcpy(mail.ucn, subpoena.lReceiverUcn);
	return mail.uclp;
}

bool TLetterTo::SpecificPreparations()
{
	subpoena.regard = rSentSubpoena->subpoenaNo;
	subpoena.execListNo = 0;
	return true;
}

bool TLetterTo::GetReceiver(TTransGroup &tUCNGroup)
{
	const char *ucnTypes = UCN_REAL_SIDES;
	const char *involvements = NULL;

	// 2016:070
	if (rSentSubpoena->subpoenaKind == SUBPOENA_KIND_EXEC_COUNTRY)
	{
		if (subpoena.subpoenaKind == SUBPOENA_KIND_EXECLET_NIATDEP)
			ucnTypes = C2S[UCN_NIA_TDEP];
		else if (subpoena.subpoenaKind == SUBPOENA_KIND_EXECLET_PRIVEXC)
		{
			ucnTypes = UCN_CITIZENS;
			involvements = C2S[INVOLVEMENT_EXECUTOR];
		}
		else if (strchr(SUBPOENA_KIND_COUNTEX_EXECLETS, subpoena.subpoenaKind))
			ucnTypes = C2S[UCN_COUNTEX];
	}

	// 2016:078
	if (*ucnTypes == UCN_NIA_TDEP && !rSentSubpoena->uclp)
	{
		msql m;

		rSentSubpoena->FloatCriteria(m, "F_NO");
		m.Add(rSentSubpoena, "F_EXEC_LIST_NO, F_EXEC_LIST_KIND");
		m.AddIsNull("F_UCLP", false);

		TQuery q(str(m));
		if (!q.Read())
		{
			m.clear();
			print_name(m, SENDER_NIA_TDEP_FOREIGN, UCN_NIA_TDEP);
			strcpy(tUCNGroup.name, str(m));
			tUCNGroup.ucnType = UCN_NIA_TDEP;
			strcpy(tUCNGroup.ucn, SENDER_NIA_TDEP_FOREIGN);
			return true;
		}
	}

	return get_ucn(pParent, &tUCNGroup, ucnTypes, true, involvements, involvements != NULL, NULL, false);
}

bool TLetterTo::PrepareForNotify(TLinkWind &wLink)
{
	SubpoenaKind->Seek(subpoena.subpoenaKind);

	if (!SubpoenaKind->LinksAllowed() && rSentSubpoena->ProxyPresent() &&
		(rSentSubpoena->proxyKind != SIDE_MARK_CONSENT_OF_INDIRECT ||
		rSentSubpoena->RedirectPresent()))
	{
		error("Не може да бъде изготвено писмо за документ, с който лицето е уведомено чрез пълномощник.");
	}
	else
	{
		wLink << rSentSubpoena;
		subpoena << wLink;

		// Fill another subpoena data
		subpoena.sideNo = sideIndex + 1;
		subpoena.eventStatus = result;
		subpoena.officiallyNotified = SUBPOENA_OFFICIALLY_NONNOTIFIED;
		strcpy(subpoena.userUcn, Default->ucn);
		subpoena.receivedStatus = SRS_SENT;

		TTransGroup tUCNGroup;

		if (SpecificPreparations() && GetReceiver(tUCNGroup))
		{
			TNamedGroup *const ucnGroup = TNamedGroup::NewGroup(tUCNGroup.ucnType);
			TVirtPtr VP(ucnGroup);

			*ucnGroup << tUCNGroup;

			if (GetAddress(ucnGroup, *SubpoenaKind->htemplate))
			{
				subpoena.lReceiverUcnType = tUCNGroup.ucnType;
				strcpy(subpoena.lReceiverUcn, tUCNGroup.ucn);
				return true;
			}
		}
	}

	return false;
}

// ----- TTelegram -----------------------------------------------------------
TTelegram::TTelegram(TWindow *parent, int sideIndex, char evStatus, TSubpoena &subpoena, TRCDKeyContainer *keyContainer)
:
	TNotifying(parent, sideIndex, evStatus, subpoena, keyContainer)
{
}

const char *TTelegram::Prefix()
{
	return "Телеграма до";
}

// ----- TStatePaper ---------------------------------------------------------
TStatePaper::TStatePaper(TWindow *parent, int sideIndex, char evStatus, TSubpoena &subpoena, TRCDKeyContainer *keyContainer)
:
	TNotifying(parent, sideIndex, evStatus, subpoena, keyContainer)
{
}

const char *TStatePaper::Prefix()
{
	return "Уведомяване чрез ДВ на";
}

bool TStatePaper::FillRegMailReceiver(TRegisteredMail &mail)
{
	TFoundGroup ucnGroup;

	if (get_ucn(pParent, &ucnGroup, UCN_REAL_SIDES) && GetAddress(&ucnGroup, false))
	{
		mail << subpoena;
		mail.ucnType = ucnGroup.ucnType;
		strcpy(mail.ucn, ucnGroup.ucn);
		return true;
	}

	return false;
}

// ----- TSWNotify -----------------------------------------------------------
TSWNotify::TSWNotify(TWindow* parent, int sideIndex, char evStatus, TSubpoena &subpoena, TRCDKeyContainer *keyContainer,
	TSubpoena *tSentSubpoena)
:
	TNotifying(parent, sideIndex, evStatus, subpoena, keyContainer)
{
	rSentSubpoena = tSentSubpoena;
}

bool TSWNotify::SpecificPreparations()
{
	switch (subpoena.subpoenaKind)
	{
		case SK_PREVIOUSLY_NOTIFIED :
		{
			if (!strchr(KIND_SESSIONS_ENDOCS, pKeyContainer->kind))
				return error("%d: kind not supported by previous notify", pKeyContainer->kind);

			TSession session;
			session.key = subpoena.key;

			if (!_select_session(pParent, &session, _KIND_V_PREV_SESSIONS, NULL))
				return false;

			subpoena.subpoenaDate = session.date;
			break;
		}
		case SK_NOTIFIED_AT_CURRENT_EVENT : subpoena.subpoenaDate = subpoena.date; break;
		case SUBPOENA_KIND_DELIVERY_NOTICE :
		{
			subpoena.regard = rSentSubpoena->subpoenaNo;
			break;
		}
	}

	return true;
}
