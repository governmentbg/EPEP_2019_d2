#include "cac.h"

TSubpoena::TSubpoena()
	: TLinkWind("T_SUBPOENA", TSubpoena::Clone), TSubServeData(this)
{
	Add(
		new TLong("F_SUBPOENA_NO", &subpoenaNo),
		new TChar("F_SUBPOENA_KIND", &subpoenaKind),
		new TChar("F_EVENT_STATUS", &eventStatus),
		new TDate("F_SUBPOENA_DATE", &subpoenaDate, true),
		new TChar("F_OFFICIALLY_NOTIFIED", &officiallyNotified),
		new TLong("F_SIDE_NO", &sideNo),
		new TLong("F_EXPENSE", &expense),
		new TLongAlias("F_UCLP", &uclp),
		new TLongAlias("F_STREET", &street),
		new TString("F_ADR_NO", adrNo, sizeof adrNo),
		new TString("F_SUB_NO", subNo, sizeof subNo),
		new TString("F_ENTRANCE", entrance, sizeof entrance),
		new TString("F_FLOOR", floor, sizeof floor),
		new TString("F_APARTMENT", apartment, sizeof apartment),
		new TString("F_BLOCK_NO", blockNo, sizeof blockNo),
		new TString("F_PHONE", phone, sizeof phone),
		new TString("F_TELEX", telex, sizeof telex),
		new TString("F_FAX", fax, sizeof fax),
		new TString("F_EMAIL", email, sizeof email),
		new TString("F_REMARK", remark, sizeof remark),
		new TString("F_DELIVERER_UCN", delivererUcn, sizeof delivererUcn),
		new TString("F_USER_UCN", userUcn, sizeof userUcn),
		new TLong("F_BY_POST", &byPost),
		new TLong("F_BY_POST_X", &byPostX),
		new TLong("F_REGARD", &regard),
		new TChar("F_LRECEIVER_UCN_TYPE", &lReceiverUcnType),
		new TString("F_LRECEIVER_UCN", lReceiverUcn, sizeof lReceiverUcn),
		new TLong("F_OUTREG_NO", &outRegNo),
		new TLong("F_OUTREG_YEAR", &outRegYear),
		new TChar("F_MULTI", &multi),
		new TChar("F_CONVERTED", &converted),
		new TStringAlias("F_COUNTRY", country, sizeof country),
		new TString("F_FOREIGN", foreign, sizeof foreign),
		new TLong("F_ANNOUNCE_NO", &announceNo),
		new TLong("F_REQUEST_ID", &requestId),
		new TLong("F_EXEC_LIST_NO", &execListNo),
		new TLong("F_EXEC_LIST_KIND", &execListKind),
		new TCoord("F_COORD_N", &coordN),	// 2015:348 was missing
		new TCoord("F_COORD_E", &coordE),	// 2015:348 was missing
		new TDate("F_VALID_UNTIL", &validUntil, false),		
		NULL
	);

	multi = SUBPOENA_SINGLE;
	// 2010:021
	subpoenaKind = SUBPOENA_KIND_NONE;
	receivedStatus = SRS_NONE;
}

TGroup *TSubpoena::Clone()
{
	return new TSubpoena;
}

void TSubpoena::ExactCriteria(msql &m, const char *fields)
{
	TBaseWind::ExactCriteria(m, fields);
	m.Add(this, "F_EVENT_STATUS, F_SUBPOENA_NO");
}

void TSubpoena::FloatCriteria(msql &m, const char *fields)
{
	TBaseWind::FloatCriteria(m, fields);
	m.Add(this, "F_EVENT_STATUS");
}

void TSubpoena::MultiCriteria(msql &m, const char *fields) const
{
	m.Begin(Name, fields);
	m.Add(this, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_EVENT_STATUS, F_SUBPOENA_NO");
	m.AddChar("F_MULTI", SUBPOENA_MULTI);
}

bool TSubpoena::Delete(bool exact)
{
	msql m;
	bool result = true;

	if (exact)
		ExactCriteria(m);
	else
		FloatCriteria(m);

	TQuery q(str(m));
	while (q.Read())
	{
		if (announceNo)
		{
			AnnounceIndex->no = announceNo;
			AnnounceIndex->year = subpoenaDate.year;
			AnnounceIndex->Delete(true);
		}

		// 2013:084 LPR/LRQ: support for subpoena electrics
		for (int type = 0; type < TROUBLE_COUNT; type++)
		{
			char name[SIZE_OF_PATH];

			electric_name(this, name, type);

			if (exist(name))
				unlock_delete_file(name, true);
		}

		if (!TLinkWind::Delete(true))
			result = false;
	}

	return result;
}

bool TSubpoena::Insert()
{
	if (subpoenaNo <= 0)
		Generate();

	return TGroup::Insert();
}

void TSubpoena::Generate()
{
	msql m;

	TBaseWind::FloatCriteria(m, "MAX(F_SUBPOENA_NO)");	// 2012:125 FIX: ignore event status
	TGetGroup g(str(m), new TLong("MAX", &subpoenaNo));

	if (++subpoenaNo == SUBPOENA_NO_MAX)
		generator_limit(Name);
}

long TSubpoena::GenerateRequestId(TCACDataBase *dataBase)
{
	return dataBase->GetNext("G_LEGAL_REQUEST_ID", "F_REQUEST_ID");
}

void TSubpoena::Print(char* const dest, const size_t destSz) const
{
	CharPtr tmp(512);

	strcpy(dest, SubpoenaKind->Seek(subpoenaKind));

	strConcat(dest, " ", destSz);
	PrintLink(tmp, tmp.Size());
	strConcat(dest, tmp, destSz);

	strConcat(dest, " ", destSz);
	PrintAddress(tmp.Str());
	strConcat(dest, tmp, destSz);
}

void TSubpoena::Print(mstr &m)
{
	char s[0x400];
	TSubpoena::Print(s, sizeof s);
	m.cat(s);
}

void TSubpoena::PrintLink(char* const dest, const size_t destSz) const
{
	side_link_to_string(dest, destSz, *this, false, false);
}

void TSubpoena::PrintProxy(char* const dest, const size_t destSz) const
{
	if (!ucnAndUcnTypeToName(proxyUcn, proxyUcnType, dest, destSz))
		*dest = '\0';
}

void TSubpoena::PrintAddress(char *s, long flags) const
{
	TAnyAddress *const address = TAnyAddress::NewGroup(any(country));
	TVirtPtr VP(address);	// 2016:203 was missing
	mstr m;

	*address << this;
	address->Print(m, flags);
	strzcpy(s, str(m), 0x100);
}

void TSubpoena::PrintAddress(mstr &m, long flags) const
{
	TAnyAddress *address = TAnyAddress::NewGroup(any(country));
	TVirtPtr VP(address);	// 2016:203 instead of delete

	*address << this;
	address->Print(m, flags);
}

bool TSubpoena::operator<=(const TSubpoena& that) const
{
	return key != that.key ? key < that.key : date != that.date ? date < that.date : kind != that.kind ? kind < that.kind :
		sideNo != that.sideNo ? sideNo < that.sideNo : strcmp(ucn, that.ucn) ? strcmp(ucn, that.ucn) < 0 :
		ucnType != that.ucnType ? ucnType < that.ucnType :
		involvement != that.involvement ? involvement < that.involvement :
		officiallyNotified != that.officiallyNotified ? officiallyNotified < that.officiallyNotified :
		subpoenaDate != that.subpoenaDate ? subpoenaDate < that.subpoenaDate : ProxyPresent() <= that.ProxyPresent();
}

bool TSubpoena::ProxyPresent() const
{
	return (proxyKind == SIDE_MARK_LIKE_LAWYER || proxyKind == SIDE_MARK_INDIRECT_AS_ANY ||
		proxyKind == SIDE_MARK_AS_WITH_INVOLVEMENT ||	// 2014:206
		proxyKind == SIDE_MARK_CONSENT_OF_INDIRECT) && Proxied();
}

bool TSubpoena::RedirectPresent() const
{
	if (Redirected())
	{
		if (redirectKind == SIDE_MARK_LIKE_LAWYER)
		{
			if (redirectUcnType && ucnPresent(redirectUcn) && redirectInvolvement)
				return true;
		}
		else
			error("invalid redirectKind %d in proxy present", redirectKind);
	}

	return false;
}

void TSubpoena::RegardCriteria(msql &m, const char *fields, bool stickers)
{
	TBaseWind::ExactCriteria(m, fields);
	m.AddChar("F_EVENT_STATUS", eventStatus);
	m.AddLong("F_REGARD", subpoenaNo);
	m.AddChars(stickers ? "F_SUBPOENA_KIND" : "NOT F_SUBPOENA_KIND", SUBPOENA_KIND_STICKERS);
}

bool TSubpoena::HasRegards()
{
	msql m;

	RegardCriteria(m, "F_REGARD", false);
	TQuery q(str(m));
	return q.Read();
}

char TSubpoena::InitialStatus() const
{
	return (byPost & OUTREG_ELECTRON) || (byPostX & OUTREG_X_ELECTRON) ? SRS_SENT : SRS_FOR_DELIVER;
}

void TSubpoena::PrintUserUcnDescr(mstr &m, const char *prefix)
{
	if (atof(userUcn))  // == true, except maybe for converted
	{
		m.cat(prefix);
		print_name(m, userUcn, UCN_CITIZEN_UCN);
	}
}

void TSubpoena::PrintWllWConverted(mstr &m)
{
	if (converted == WLLW_CONVERTED)
		m.cat("\r\nОт конвертиране");
}

void TSubpoena::PrintSentDescr(mstr &m) const
{
	if (print_postal(m, byPost, byPostX))
		m.cat(" ");

	if (outRegNo)
	{
		m.printf(" с изх.No %ld/", outRegNo);

		TOutReg outReg;

		outReg.key.no = outRegNo;
		outReg.key.year = outRegYear;

		if (outReg.Try("F_DATE"))
			outReg.date.Print(m, C_DATE_GOD);
		else
			m.printf("%ldг.", outRegNo);

		m.cat(" ");
	}

	subpoenaDate.Print(m, "на %Dг.");
}

static bool same_linkage(const TLinkWind *linkWind, const TLinkWind *tLinkWind)
{
	return linkWind->involvement == tLinkWind->involvement && linkWind->proxyUcnType == tLinkWind->proxyUcnType &&
		strcmp(linkWind->proxyUcn, tLinkWind->proxyUcn) == 0 &&
		linkWind->proxyInvolvement == tLinkWind->proxyInvolvement && linkWind->proxyKind == tLinkWind->proxyKind &&
		linkWind->redirectUcnType == tLinkWind->redirectUcnType &&
		strcmp(linkWind->redirectUcn, tLinkWind->redirectUcn) == 0 &&
		linkWind->redirectInvolvement == tLinkWind->redirectInvolvement &&
		linkWind->redirectKind == tLinkWind->redirectKind;
}

void TSubpoena::PrintNotifieds(mstr &m, TGArray<TLinkWind> *pLinks, const char *sidesep, const char *invlsep)
{
	TGArray<TLinkWind> links;

	if (!invlsep)
		invlsep = sidesep;

	for (int i = 0; i < pLinks->Count(); i++)
		links.Add((*pLinks)[i]);

	if (!links.Count())
		links.Add(this);

	for (;;)
	{
		TGArray<TLinkWind> tLinks;
		TLinkWind *linkWind = links.Detach(0);

		tLinks.Add(linkWind);

		int index = 0;
		while (index < links.Count())
		{
			TLinkWind *tLinkWind = links[index];

			if (same_linkage(tLinkWind, linkWind))
			{
				tLinks.Add(tLinkWind);
				links.Detach(index);
			}
			else
				index++;
		}

		m.cat("До ");
		print_side_links(m, linkWind, false, true, &tLinks);

		if (linkWind->Proxied() && linkWind->proxyKind == SIDE_MARK_AS_WITH_INVOLVEMENT)
			; // 2014:226 TRQ/LPR/LRQ: involvements already shown, skip lead involvement
		else
			m.printf("%sкато %s", invlsep, Involvement->Seek(linkWind->involvement));

		tLinks.Clear();
		if (links.Count())
			m.cat(sidesep);
		else
			break;
	}
}

void TSubpoena::PrintNotifieds(mstr &m, const char *sidesep, const char *invlsep)
{
	TGArray<TLinkWind> links;

	// 2008:179 LPR: load sub-poenas
	if (multi == SUBPOENA_LEADER)
	{
		msql m;
		TLinkWind linkWind;

		MultiCriteria(m);
		linkWind.LoadArray(links, str(m));
	}

	PrintNotifieds(m, &links, sidesep, invlsep);
}

void TSubpoena::PrintDescription(mstr &m)
{
	PrintNotifieds(m, "\r\n");

	if (multi == SUBPOENA_MULTI)
		m.cat(" * Част от групова призовка *");

	SubpoenaKind->Seek(subpoenaKind);

	switch (SubpoenaKind->proc)
	{
		case SK_PROC_NULL :
		{
			m.cat("\r\nНеподходящ вид уведомяване");
			PrintWllWConverted(m);
			break;
		}

		case SK_PROC_LETTER_TO :
		case SK_PROC_STANDALONE_LETTER :
		{
			if (regard)
			{
				m.printf("\r\n%s (във връзка с", SubpoenaKind->name);

				TSubpoena tSubpoena;

				tSubpoena << this;
				tSubpoena.subpoenaNo = regard;

				if (tSubpoena.Try("F_SUBPOENA_KIND"))
					m.printf(" %s", SubpoenaKind->Seek(tSubpoena.subpoenaKind));

				m.printf(" No %ld)", regard);
			}

			m.cat("\r\nИзпратено ");
			PrintSentDescr(m);

			if (lReceiverUcnType && atof(lReceiverUcn))
			{
				m.cat("\r\nДо ");
				print_name(m, lReceiverUcn, lReceiverUcnType);

				TAddress address;

				address.ucnType = lReceiverUcnType;
				strcpy(address.ucn, lReceiverUcn);

				if (address.Try("F_UCLP"))
					m.printf(", %s", UCLP->Seek(address.uclp));
			}
			else if (any(receivedText))
				m.printf("\r\n%s", receivedText);

			PrintUserUcnDescr(m, "\r\nИзготвил: ");

			break;
		}

		case SK_PROC_NOTIFYING:
		{
			m.printf("\r\n\"%s\" ", SubpoenaKind->name);
			m.cat(strchr(SUBPOENA_KIND_WILL_BES, subpoenaKind) ? "от " : "на ");
			subpoenaDate.Print(m, C_DATE_GOD);

			PrintUserUcnDescr(m, "\r\nУведомил: ");
			PrintWllWConverted(m);

			break;
		}

		default:
		{
			m.cat("\r\n");
			PrintAddress(m);
			m.printf("\r\n%s", SubpoenaKind->name);
			if (announceNo || execListNo)
				m.printf(" No %ld/%ldг.", announceNo ? announceNo : execListNo, subpoenaDate.year);
			//m.printf("\r\n%s ", ReceivedStatus->Seek(receivedStatus == SRS_SENT ? SRS_SENT : SRS_CREATED));
			char initialStatus = strchr(SRS_INITIALS, receivedStatus) ? receivedStatus : InitialStatus();
			m.printf("\r\n%s ", ReceivedStatus->Seek(initialStatus));
			PrintSentDescr(m);

			if (Received())
			{
				m.printf("\r\n%s", ReceivedStatus->Seek(receivedStatus));
				subpoenaReceived.Print(m, " на %D");
				receivedTime.Print(m, " %T");
				m.sep_cat(" ", receivedText);
			}

			if (atof(delivererUcn))
			{
				m.cat("\r\nПризовкар: ");
				print_name(m, delivererUcn, UCN_CITIZEN_UCN);
			}

			if (!subpoenaReturned.Empty())
			{
				m.cat("\r\nВърнат(а,о) в деловодството на ");
				subpoenaReturned.Print(m, C_DATE_GOD);
			}

			PrintUserUcnDescr(m, "\r\nИзготвил: ");
			PrintWllWConverted(m);
		}
	}
}

bool TSubpoena::AskDelete() const
{
	if (Received())
		return error("Има попълнени данни за връчване.");

	if (converted == WLLW_CONVERTED)
		return error("Елементът е от конвертиране.");

	if (announceNo)	// 2012:045 LRQ
		return ask("Избраният документ има номер. Желаете ли да продължите?");

	long postal = byPost & OUTREG_POSTAL;

	return ask("%sЖелаете ли да изтриете избрания елемент?",
		postal == OUTREG_R || postal == OUTREG_AR || postal == OUTREG_PACKAGE ?
		"Има данни за изпращане по пощата - необходимо е да ги изтриете ръчно "
		"от режим \"Подготовка списък на изходяща поща\". " : "");
}

bool TSubpoena::FindOrigin(TRCDKey &origin) const
{
	if (strchr(TYPE_ORIGINS, key.type))
	{
		origin = key;
		return true;
	}

	if (key.type == TYPE_INREG || key.type == TYPE_OUTREG)
	{
		const char *name = key.type == TYPE_INREG ? "T_INREG" : "T_OUTREG";
		msql m(name, "F_LAWSUIT_NO, F_LAWSUIT_YEAR, F_LAWSUIT_TYPE");

		m.AddLong("F_NO", key.no);
		m.AddLong("F_YEAR", key.year);

		TQuery q(str(m));
		if (q.Read())
		{
			TIOReg ioReg(name, NULL);

			ioReg << q;
			// 2014:077 LPR/LRQ: use key if lawsuit is empty
			origin = ioReg.lawsuit.type ? ioReg.lawsuit : key;
			return true;
		}
		else
		{
			say_no_data(name);
			failure();
		}
	}

	return false;
}
