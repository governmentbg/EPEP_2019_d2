// ----- TEisAlias -----------------------------------------------------------
TEisAlias::TEisAlias(const char *name, long *data)
	: TLongAlias(name, data)
{
}

void TEisAlias::ExportToControl(TControl *control) const
{
	TYPECHECK_PTR_CAST(TEisAliasFace, control)->SetL(L());
}

// ----- MPP -----------------------------------------------------------------
MPP::MPP()
	: TXMLGroup("MPP")
{
	attribs.Add(
		new TXMLDate("mppdta", &dta),
		new TXMLMoney("mpprzm", &rzm),
		new TXMLSid("mppsid", &sid),
		new TXMLInt("mppste", &ste),
		new TXMLInt("mppstr", &str),
		new TXMLInt("mppvid", &vid),
		NULL
	);

	Apply("mpprzm", XML_NULLABLE);

	Add(
		new TDate("mppdta", &dta, false),
		new TMoney("mpprzm", &rzm),
		new TLong("mppsid", &sid),
		new TEisAlias("mppste", &ste),
		new TEisAlias("mppstr", &str),
		new TEisAlias("mppvid", &vid),
		NULL
	);
}

// ----- ADR -----------------------------------------------------------------
ADR::ADR()
	: TXMLObject("ADR")
{
	attribs.Add(
		new TXMLSid("adrsid", &sid),
		new TXMLInt("adrtip", &tip),
		new TXMLInt("adrdrj", &drj),
		new TXMLInt("adrnsmbgr", &nsmbgr),
		new TXMLInt("adrrjn", &rjn),
		new TXMLInt("adrpstkod", &pstkod),
		new TXMLInt("adrmstvid", &mstvid),
		new TXMLInt("adrkodpdl", &kodpdl),
		new TXMLString("adrnsmchj", nsmchj, sizeof nsmchj),
		new TXMLString("adrkrdtxt", krdtxt, sizeof krdtxt),
		new TXMLString("adrnmr", nmr, sizeof nmr),
		new TXMLString("adrblk", blk, sizeof blk),
		new TXMLString("adrvhd", vhd, sizeof vhd),
		new TXMLString("adretj", etj, sizeof etj),
		new TXMLString("adrapr", apr, sizeof apr),
		new TXMLString("adrmsttxt", msttxt, sizeof msttxt),
		new TXMLInt("adrloc", &loc),
		NULL
	);
};

// ----- NKZ -----------------------------------------------------------------
NKZ::NKZ()
	: TXMLGroup("NKZ")
{
	attribs.Add(
		new TXMLInt("iptsrkden", &iptsrkden),
		new TXMLDate("iptsrkdta", &iptsrkdta),
		new TXMLInt("iptsrkgdn", &iptsrkgdn),
		new TXMLInt("iptsrkmsc", &iptsrkmsc),
		new TXMLInt("iptsrksdc", &iptsrksdc),
		new TXMLInt("nkzakt", &akt),
		new TXMLDate("nkzaktdta", &aktdta),
		new TXMLInt("nkzden", &den),
		new TXMLInt("nkzgdn", &gdn),
		new TXMLInt("nkzmsc", &msc),
		new TXMLInt("nkzncn", &ncn),
		new TXMLInt("nkzrjm", &rjm),
		new TXMLMoney("nkzrzm", &rzm),
		new TXMLInt("nkzsdc", &sdc),
		new TXMLSid("nkzsid", &sid),
		new TXMLInt("nkzstr", &str),
		new TXMLInt("nkztip", &tip),
		new TXMLInt("nkzvid", &vid),
		NULL
	);

	Apply("nkzrzm", XML_NULLABLE);
	Apply("nkzrjm", XML_NULLABLE);
	Apply("nkzncn", XML_NULLABLE);

	Add(
		new TLong("iptsrkden", &iptsrkden),
		new TDate("iptsrkdta", &iptsrkdta, false),
		new TLong("iptsrkgdn", &iptsrkgdn),
		new TLong("iptsrkmsc", &iptsrkmsc),
		new TLong("iptsrksdc", &iptsrksdc),
		new TEisAlias("nkzakt", &akt),
		new TDate("nkzaktdta", &aktdta, false),
		new TLong("nkzden", &den),
		new TLong("nkzgdn", &gdn),
		new TLong("nkzmsc", &msc),
		new TEisAlias("nkzncn", &ncn),
		new TEisAlias("nkzrjm", &rjm),
		new TMoney("nkzrzm", &rzm),
		new TLong("nkzsdc", &sdc),
		new TLong("nkzsid", &sid),
		new TEisAlias("nkzstr", &str),
		new TEisAlias("nkztip", &tip),
		new TEisAlias("nkzvid", &vid),
		NULL
	);
}

void NKZ::Print(mstr &m)
{
	m.cpy(EisVal->Seek(nmk_nkzvid, vid));
	m.printf(" - %s", EisVal->Seek(nmk_nkztip, tip));
}

// ----- IZPNKZ --------------------------------------------------------------
IZPNKZ::IZPNKZ()
	: TXMLObject("IZPNKZ")
{
	attribs.Add(
		new TXMLDate("izpdta", &dta),
		new TXMLSid("izpnkzsid", &nkzsid),
		new TXMLInt("izpste", &ste),
		new TXMLInt("izpsteprc", &steprc),
		new TXMLInt("izpstr", &str),
		NULL
	);
}

// ----- NPRFZLSTA -----------------------------------------------------------
NPRFZLSTA::NPRFZLSTA()
	: TXMLObject("NPRFZLSTA")
{
	attribs.Add(
		new TXMLSid("fzlsid", &sid),
		new TXMLDate("nprfzldta", &dta),
		new TXMLInt("nprfzlkcv", &kcv),
		new TXMLInt("nprfzlosn", &osn),
		new TXMLInt("nprfzlsts", &sts),
		NULL
	);

	Apply("fzlsid", XML_REQUIRED);
}

// ----- NPL -----------------------------------------------------------------
NPL::NPL(TGroup *parent)
	: TXMLObject("NPL")
{
	attribs.Add(
		new TXMLInt("nplalk", &alk),
		new TXMLInt("nplbnz", &bnz),
		new TXMLInt("npljiv", &jiv),
		new TXMLInt("npllekpsx", &lekpsx),
		new TXMLInt("npllrknrk", &lrknrk),
		new TXMLInt("nplmlt", &mlt),
		new TXMLInt("nplmltot", &mltot),
		new TXMLInt("nplnkzpdm", &nkzpdm),
		new TXMLInt("nplnrk", &nrk),
		new TXMLInt("nplnrkvid", &nrkvid),
		new TXMLInt("nplotc", &otc),
		new TXMLInt("nplpneprc", &pneprc),
		new TXMLInt("nplpro", &pro),
		new TXMLInt("nplprs", &prs),
		new TXMLInt("nplrzpnrk", &rzpnrk),
		new TXMLSid("nplsid", &sid),
		new TXMLInt("nplskt", &skt),
		new TXMLInt("nplspi", &spi),
		new TXMLInt("nplucl", &ucl),
		new TXMLInt("npluclprc", &uclprc),
		new TXMLInt("nplusljiv", &usljiv),
		new TXMLInt("nplvmk", &vmk),
		new TXMLInt("nplvui", &vui),
		new TXMLInt("nplxms", &xms),
		new TXMLInt("nplzdrste", &zdrste),
		NULL
	);

	parent->Add(
		new TEisAlias("nplalk", &alk),
		new TEisAlias("nplbnz", &bnz),
		new TEisAlias("npljiv", &jiv),
		new TEisAlias("npllekpsx", &lekpsx),
		new TEisAlias("npllrknrk", &lrknrk),
		new TEisAlias("nplmlt", &mlt),
		new TEisAlias("nplmltot", &mltot),
		new TEisAlias("nplnkzpdm", &nkzpdm),
		new TEisAlias("nplnrk", &nrk),
		new TEisAlias("nplnrkvid", &nrkvid),
		new TEisAlias("nplotc", &otc),
		new TEisAlias("nplpneprc", &pneprc),
		new TEisAlias("nplpro", &pro),
		new TEisAlias("nplprs", &prs),
		new TEisAlias("nplrzpnrk", &rzpnrk),
		new TLong("nplsid", &sid),
		new TEisAlias("nplskt", &skt),
		new TEisAlias("nplspi", &spi),
		new TEisAlias("nplucl", &ucl),
		new TEisAlias("npluclprc", &uclprc),
		new TEisAlias("nplusljiv", &usljiv),
		new TEisAlias("nplvmk", &vmk),
		new TEisAlias("nplvui", &vui),
		new TEisAlias("nplxms", &xms),
		new TEisAlias("nplzdrste", &zdrste),
		NULL
	);
}

// ----- SBC -----------------------------------------------------------------
SBC::SBC()
	: TXMLObject("SBC")
{
	attribs.Add(
		new TXMLInt("sbcetn", &etn),
		new TXMLInt("sbcgrj", &grj),
		new TXMLInt("sbcobr", &obr),
		new TXMLInt("sbcple", &ple),
		new TXMLInt("sbcrcd", &rcd),
		new TXMLSid("sbcsid", &sid),
		new TXMLInt("sbcspj", &spj),
		new TXMLInt("sbctrd", &trd),
		new TXMLInt("sbcznq", &znq),
		new TXMLInt("sbcrge", &rge),
		NULL
	);
}

// ----- VSL -----------------------------------------------------------------
VSL::VSL(TGroup *parent)
	: TXMLObject("VSL")
{
	attribs.Add(
		new TXMLInt("vsldlj", &dlj),
		new TXMLInt("vslktg", &ktg),
		new TXMLInt("vslpdl", &pdl),
		new TXMLSid("vslsid", &sid),
		new TXMLInt("vslzvn", &zvn),
		NULL
	);

	parent->Add(
		new TEisAlias("vsldlj", &dlj),
		new TEisAlias("vslktg", &ktg),
		new TEisAlias("vslpdl", &pdl),
		new TLong("vslsid", &sid),
		new TEisAlias("vslzvn", &zvn),
		NULL
	);
}

// ----- TEisGroup -----------------------------------------------------------
TEisGroup::TEisGroup(const char *xmlName, const char *groupName, TGroup *(*tClone)())
	: TXMLGroup(xmlName, groupName, tClone)
{
	Add(
		NEW_RCDKEY("F_", &key),
		NULL
	);
}

void TEisGroup::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(TGroup::Name, fields);
	m.Add(this, "F_NO, F_YEAR, F_TYPE");
}

void TEisGroup::LoadFloat(TXMLArray *array, TEisGroup *group, TXMLArray *descript)
{
	msql m;

	*this << group;
	TEisGroup::FloatCriteria(m);
	LoadArray(array, str(m), descript);
}

// ----- FZL -----------------------------------------------------------------
FZL::FZL()
	: TEisGroup("FZL", "T_FZL", FZL::Clone), nprfzlsta(new NPRFZLSTA), sid(nprfzlsta->sid), dirty(false)
{
	Init();
}

FZL::FZL(const char *xmlName, TGroup *(*tClone)())
	: TEisGroup(xmlName, "T_FZL", tClone), nprfzlsta(new NPRFZLSTA), sid(nprfzlsta->sid)
{
	Init();
}

void FZL::Init()
{
	mpp = new TXARRAY(MPP);
	izpnkz = new IZPNKZ;
	nkz = new TXARRAY(NKZ);
	npl = new NPL(this);
	vsl = new VSL(this);

	objects.Add(mpp, izpnkz, nkz, nprfzlsta, npl, vsl, NULL);

	// 2012:320 LPR: -> dextro desc
	attribs.Add(
		new TXMLSid("fzlsid", &sid, SID_FZL),
		new TXMLInt("fzlgrjbgr", &grjbgr),
		new TXMLInt("fzlgrjchj", &grjchj),
		new TXMLInt("fzlpol", &pol),
		new TXMLString("fzlegn", egn, sizeof egn),
		new TXMLString("fzllnc", lnc, sizeof lnc),
		new TXMLString("fzlime", ime, sizeof ime),
		new TXMLString("fzlprz", prz, sizeof prz),
		new TXMLString("fzlfma", fma, sizeof fma),
		new TXMLString("fzlimecyr", imecyr, sizeof imecyr),
		new TXMLDate("fzldtarjd", &dtarjd),
		NULL
	);

	Apply("*", XML_NULLABLE);
	Apply("fzlgrjbgr", XML_INCLUDED, "8805");
	Apply("fzlpol", XML_INCLUDED, "691, 706");

	attribs.Add(
		new TXMLString("fzlimelat", imelat, sizeof imelat),
		new TXMLDate("fzldtapnl", &dtapnl),
		NULL);

	Apply("*", XML_INCLUDED);

	// FZL
	Add(
		new TLong("fzlsid", &sid),
		new TEisAlias("fzlgrjbgr", &grjbgr),
		new TEisAlias("fzlgrjchj", &grjchj),
		new TEisAlias("fzlpol", &pol),
		new TString("fzlegn", egn, sizeof egn),
		new TString("fzllnc", lnc, sizeof lnc),
		new TString("fzlime", ime, sizeof ime),
		new TString("fzlprz", prz, sizeof prz),
		new TString("fzlfma", fma, sizeof fma),
		new TString("fzlimecyr", imecyr, sizeof imecyr),
		new TString("fzlimelat", imelat, sizeof imelat),
		new TTimeStamp("fzldtarjd", &dtarjd),
		new TDate("F_DTARJD", &dtarjd, false),
		new TTimeStamp("fzldtapnl", &dtapnl),
		new TDate("F_DTAPNL", &dtapnl, false),
		NULL
	);
	// IZPNKZ
	Add(
		new TDate("izpdta", &izpnkz->dta, false),
		new TLong("izpnkzsid", &izpnkz->nkzsid),
		new TEisAlias("izpste", &izpnkz->ste),
		new TEisAlias("izpsteprc", &izpnkz->steprc),
		new TEisAlias("izpstr", &izpnkz->str),
		NULL
	);
	// NPRFZLSTA
	Add(
		new TDate("nprfzldta", &nprfzlsta->dta, false),
		new TEisAlias("nprfzlkcv", &nprfzlsta->kcv),
		new TEisAlias("nprfzlosn", &nprfzlsta->osn),
		new TEisAlias("nprfzlsts", &nprfzlsta->sts),
		NULL
	);

	descs = Descs + TABLE_FZL;
}

TGroup *FZL::Clone()
{
	return new FZL();
}

void FZL::ExactCriteria(msql &m, const char *fields)
{
	FloatCriteria(m, fields);
	m.Add(this, "fzlsid");
}

const char *FZL::Invalid()
{
	const char *s = NULL;

	PrintImeCyr();

	if (!grjbgr && !grjchj)
		s = "Не е въведено гражданство.";
	else if (!dtapnl.Empty() && dtapnl < dtarjd)
		s = "Датата на смъртта е преди рожденната дата.";
	else if (grjbgr)
	{
		if (!any(egn))
			s = "ЕГН е задължителен за български граждани.";
		else if (!any(ime) || !any(fma))
			s = "Име и фамилия са задължителни за български граждани.";
	}
	else
	{
		if (!any(imecyr))
			s = "Имена на кирилица са задължителни за чужди граждани.";
		else if (!pol)
			s = "Полът е задължителен за чужди граждани.";
		// 2013:214 FIX: any(lnc) -> any(egn)
		else if (!any(egn) && dtarjd.Empty())
			s = "Датата на раждане е задължителна при липса на ЕГН.";
	}

	struct TNameCheck
	{
		const char *value;
		sign cyrillic;
		const char *errorMsg;
	};

	TNameCheck NameChecks[] =
	{
		{ ime, true,  "Невалидно име." },
		{ prz, true, "Невалидно презиме." },
		{ fma, true,  "Невалидна фамилия." },
		{ imecyr, -1, "Невалидни имена на кирилица." },
		{ imelat, false, "Невалидни имена на латиница." },
		{ NULL, 0, NULL }
	};

	for (TNameCheck *check = NameChecks; check->value; check++)
	{
		if (!fzl_name_valid(check->cyrillic, check->value))
		{
			s = check->errorMsg;
			break;
		}
	}

	return s;
}

void FZL::LoadCitizen(const TCitizen *citizen)
{
	if (is_real_ucn(citizen))
	{
		CDate date(citizen->ucn);

		if (date.ValidEx())
		{
			strcpy(egn, citizen->ucn);
			(CDate &) dtarjd = date;
			pol = egn[8] % 2 ? pol_jena : pol_maj;
		}
	}

	strcpy(lnc, citizen->lnc);
	strcpy(ime, citizen->name);
	strcpy(prz, citizen->reName);
	strcpy(fma, citizen->family);

	if (any(citizen->reFamily))
		scprintf(fma, "-%s", citizen->reFamily);

	if (!strcmp(citizen->country, BUILDIN_COUNTRY) || !strcmp(citizen->reCountry, BUILDIN_COUNTRY))
		grjbgr = grj_BGR;
	else
		PrintImeCyr();

	if (any(citizen->country) && strcmp(citizen->country, BUILDIN_COUNTRY))
	{
		Country->Seek(citizen->country);
		grjchj = Country->eispp;
	}
	else if (any(citizen->reCountry) && strcmp(citizen->reCountry, BUILDIN_COUNTRY))
	{
		Country->Seek(citizen->reCountry);
		grjchj = Country->eispp;
	}
}

void FZL::Print(mstr &m)
{
	m.cpy(imecyr);

	if (!any(m))
	{
		m.cat(ime);

		if (any(prz))
			m.printf(" %s", prz);

		m.printf(" %s", fma);
	}
}

bool FZL::SaveCitizen(TCitizen *citizen)
{
	TCitizen tCitizen;
	bool found = false;

	if (any(egn))
	{
		strcpy(citizen->ucn, egn);
		citizen->ucnType = UCN_CITIZEN_UCN;
		found = citizen->Try();
	}
	else if (any(lnc))
	{
		msql m(citizen->Name);
		m.AddString("F_LNC", lnc);
		TQuery q(str(m));

		if (q.Read())
		{
			*citizen << q;

			if (q.Read())
				*citizen << tCitizen;	// clear
			else
				found = true;
		}
	}

	if (any(ime) && any(fma))
	{
		strzcpy(tCitizen.name, ime, sizeof tCitizen.name);

		if (any(prz))
			strzcpy(tCitizen.reName, prz, sizeof tCitizen.reName);

		if (!SaveFamily(&tCitizen, fma))
			return found;
	}
	else
	{
		char s[SIZE_OF_EISPP_STRING];
		const char *p;
		unsigned words;

		strcpy(s, imecyr);
		words = split_words(s);

		if (words < 2)
		{
			error("%s: невалидно име на FZL.", imecyr);
			return found;
		}

		strzcpy(tCitizen.name, s, sizeof tCitizen.name);
		p = s + strlen(s) + 1;

		if (words >= 3)
		{
			strzcpy(tCitizen.reName, p, sizeof tCitizen.reName);
			p += strlen(p) + 1;
		}

		if (words >= 4)
		{
			strzcpy(tCitizen.family, p, sizeof tCitizen.family);
			strzcpy(tCitizen.reFamily, p + strlen(p) + 1, sizeof tCitizen.reFamily);
		}
		else if (!SaveFamily(&tCitizen, p))
			return found;
	}

	bool bulgarian = grjbgr == grj_BGR;

	if (bulgarian)
		strcpy(tCitizen.country, BUILDIN_COUNTRY);

	if (grjchj)
	{
		msql m(Country->Name, "F_COUNTRY");
		m.AddLong("F_EISPP", grjchj);
		TQuery q(str(m));

		if (q.Read())
		{
			*Country << q;
			strcpy(bulgarian ? tCitizen.reCountry : tCitizen.country, Country->country);
		}
	}
	else if (!bulgarian)
	{
		mstr m;

		Print(m);
		error("%s: няма гражданство.", str(m));
		return found;
	}

	if (!found)
	{
		*citizen << tCitizen;
		strcpy(citizen->ucn, egn);
		citizen->ucnType = any(egn) ? UCN_CITIZEN_UCN : UCN_CITIZEN_CODE;
		return citizen->Insert();
	}

	enum { N = 6 };
	char *fs[N] = { tCitizen.name, tCitizen.reName, tCitizen.family, tCitizen.reFamily, tCitizen.country, tCitizen.reCountry };
	char *cs[N] = { citizen->name, citizen->reName, citizen->family, citizen->reFamily, citizen->country, citizen->reCountry };
	int i;

	for (i = 0; i < N; i++)
		if (strcmp(fs[i], cs[i]))
			break;

	if (i < N)
	{
		for (i = 0; i < N; i++)
			strcpy(cs[i], fs[i]);

		citizen->Update();
	}

	return true;
}

void FZL::PrintImeCyr()
{
	if (!any(imecyr))
	{
		mstr m;
		Print(m);
		strcpy(imecyr, str(m));
	}
}

bool FZL::SaveFamily(TCitizen *citizen, const char *family) const
{
	char s[SIZE_OF_EISPP_STRING];
	unsigned words;

	strcpy(s, family);
	words = split_words(s, "-");
	strzcpy(citizen->family, s, sizeof citizen->family);

	if (!words)
		return error("%s: невалидна фамилия на FZL.", fma);

	if (words > 1)
		strzcpy(citizen->reFamily, s + strlen(s) + 1, sizeof citizen->reFamily);

	return true;
}

// ----- SBH -----------------------------------------------------------------
SBH::SBH()
	: TXMLObject("SBH")
{
	attribs.Add(
		new TXMLSid("sbhsid", &sid),
		new TXMLInt("sbhstn", &stn),
		new TXMLString("sbhtxt", txt, sizeof txt),
		new TXMLInt("sbhvid", &vid),
		NULL
	);
}

// ----- SRK -----------------------------------------------------------------
SRK::SRK()
	: TXMLObject("SRK")
{
	attribs.Add(
		new TXMLShort("srkden", &den),
		new TXMLDate("srkdta", &dta),
		new TXMLDate("srkdtadod", &dtadod),
		new TXMLShort("srkgdn", &gdn),
		new TXMLShort("srkmsc", &msc),
		new TXMLShort("srkobt", &obt),
		new TXMLShort("srksdc", &sdc),
		new TXMLSid("srksid", &sid),
		new TXMLInt("srkvid", &vid),
		NULL
	);
}

void SRK::Force()
{
	Apply("srkvid", XML_REQUIRED, "1029");
	Apply("*", XML_INCLUDED | XML_EDITABLE);
	flags = XML_INCLUDED | XML_REQUIRED; 
}

// ----- DVJDLO --------------------------------------------------------------
DVJDLO::DVJDLO()
	: TXMLObject("DVJDLO")
{
	attribs.Add(
		new TXMLSid("dvjdlosid", &sid),
		new TXMLInt("dvjprc", &prc),
		new TXMLDate("dvjdta", &dta),
		new TXMLInt("dvjstripr", &stripr),
		new TXMLInt("dvjstrplc", &strplc),
		new TXMLInt("dvjvid", &vid),
		new TXMLString("dvjdloopi", opi, sizeof opi),
		new TXMLString("dvjdlodvn", dvn, sizeof dvn),
		NULL
	);
}

// ----- NPRPNESTA -----------------------------------------------------------
NPRPNESTA::NPRPNESTA()
	: TXMLObject("NPRPNESTA")
{
	attribs.Add(
		new TXMLDate("nprpnedta", &dta),
		new TXMLInt("nprpnests", &sts),
		NULL
	);
}

// ----- PNESTA --------------------------------------------------------------
PNESTA::PNESTA()
	: TXMLRequiredObject("PNESTA")
{
	attribs.Add(
		new TXMLInt("pnekcq", &kcq),
		new TXMLInt("pnestpdvs", &stpdvs),
		new TXMLInt("pnests", &sts),
		NULL
	);

	Apply("*", XML_REQUIRED);
}

// ----- DLOSTA --------------------------------------------------------------
DLOSTA::DLOSTA()
	: TXMLObject("DLOSTA")
{
	attribs.Add(
		new TXMLSid("dlostasid", &sid),
		new TXMLDate("dlodtasts", &dtasts),
		new TXMLInt("dlosts", &sts),
		NULL
	);
}

// ----- TNmrGroup -----------------------------------------------------------
TNmrGroup::TNmrGroup(const char *xmlName, const char *groupName, TGroup *(*tClone)(), const char *tNmrName, char tLetter)
	: TEisGroup(xmlName, groupName, tClone), nmrName(tNmrName), letter(tLetter)
{
	attribs.Add(new TXMLString(nmrName, nmr, sizeof nmr));

	Add(
		new TString(nmrName, nmr, sizeof nmr),
		NULL
	);
}

bool TNmrGroup::Insert()
{
	if (!any(nmr))
	{
		mstr max;
		max.printf("CAST(MAX(%s) AS CHAR(%d)) AS F_MAX", nmrName, LENGTH_OF_EISPP_NMR);
		msql m(TGroup::Name, str(max));
		long seqn;
		char tNmr[SIZE_OF_EISPP_NMR];

		sprintf(nmr, "%s%03d_____%c%%", COURT_TLACR, key.year - 1800, letter);
		m.AddLike(nmrName, nmr, false);
		TGroup nmrGroup("T_NMR_GROUP", NULL);
		TQuery q(str(m));

		q.Read();
		nmrGroup.Add(new TString("F_MAX", tNmr, sizeof tNmr));
		nmrGroup << q;
		seqn = any(tNmr) ? atoi(tNmr + 6) : 0;

		if (++seqn > SID_MAXIMUM)
		{
			return error("%s: достигнат е най-големия номер (%d). Необходимо е ръчно въвеждане на номер.", 
				TGroup::Name, SID_MAXIMUM);
		}

		sprintf(nmr + 6, "%05d%c", seqn, letter);
		calc_eispp(nmr, nmr + LENGTH_OF_EISPP_MULTI);
	}

	return TEisGroup::Insert();
}

// ----- PNE_ZVK -------------------------------------------------------------
PNE_ZVK::PNE_ZVK(const char *xmlName, TGroup *(*const tClone)())
	: TNmrGroup(xmlName ? xmlName : "PNE", "T_PNE", tClone ? tClone : PNE_ZVK::Clone, "pnenmr", EISPP_PNE_LOCAL)
{
	attribs.Add(new TXMLSid("pnesid", &sid, SID_PNE));

	Apply("*", XML_INCLUDED);
	Apply("pnesid", XML_REQUIRED);

	Add(new TLong("pnesid", &sid));

	descs = Descs + TABLE_PNE;
}

TGroup *PNE_ZVK::Clone()
{
	return new PNE_ZVK;
}

void PNE_ZVK::ExactCriteria(msql &m, const char *fields)
{
	FloatCriteria(m, fields);
	m.Add(this, "pnesid");
}

// ----- PNE -----------------------------------------------------------------
PNE::PNE()
	: PNE_ZVK("PNE", PNE::Clone), dirty(false)
{
	Init();
	objects.Add(adr, nprpnesta, pnesta, NULL);
}

PNE::PNE(const char *xmlName, TGroup *(*const tClone)())
	: PNE_ZVK(xmlName, tClone)
{
	Init();
}

void PNE::Init()
{
	adr = new ADR;
	nprpnesta = new NPRPNESTA;
	pnesta = new PNESTA;

	attribs.Add(
		new TXMLInt("pnebrjnzv", &brjnzv),
		new TXMLInt("pnebrjrne", &brjrne),
		new TXMLInt("pnebrjubt", &brjubt),
		new TXMLDate("pnedtadod", &dtadod),
		new TXMLDate("pnedtaotd", &dtaotd),
		new TXMLInt("pneotdtip", &otdtip),
		new TXMLMoney("pnestinsn", &stinsn),
		new TXMLMoney("pnestiobz", &stiobz),
		new TXMLMoney("pnestivst", &stivst),
		new TXMLString("pnestv", stv, sizeof stv),
		new TXMLString("pnetxt", txt, sizeof txt),
		new TXMLInt("pnechs", &chs),
		NULL
	);

	Apply("*", XML_INCLUDED);
	Apply("pneotdtip", XML_REQUIRED, "1629...1631");
	Apply("pnedtaotd", XML_REQUIRED);

	// PNE
	Add(
		new TLong("pnebrjnzv", &brjnzv),
		new TLong("pnebrjrne", &brjrne),
		new TLong("pnebrjubt", &brjubt),
		new TDateTime("pnedtadod", &dtadod),
		new TDate("F_DTADOD", &dtadod, false),
		new TDateTime("pnedtaotd", &dtaotd),
		new TDate("F_DTAOTD", &dtaotd, false),
		new TString("F_PNENMR", nmr, sizeof nmr),
		new TEisAlias("pneotdtip", &otdtip),
		new TMoney("pnestinsn", &stinsn),
		new TMoney("pnestiobz", &stiobz),
		new TMoney("pnestivst", &stivst),
		new TString("pnestv", stv, sizeof stv),
		new TString("pnetxt", txt, sizeof txt),
		new TLong("pnechs", &chs),
		NULL
	);
	// ADR
	Add(
		new TLong("adrsid", &adr->sid),
		new TEisAlias("adrtip", &adr->tip),
		new TEisAlias("adrdrj", &adr->drj),
		new TEisAlias("adrnsmbgr", &adr->nsmbgr),
		new TEisAlias("adrrjn", &adr->rjn),
		new TLong("adrpstkod", &adr->pstkod),
		new TEisAlias("adrmstvid", &adr->mstvid),
		new TLong("adrkodpdl", &adr->kodpdl),
		new TString("adrnsmchj", adr->nsmchj, sizeof adr->nsmchj),
		new TString("adrkrdtxt", adr->krdtxt, sizeof adr->krdtxt),
		new TString("adrnmr", adr->nmr, sizeof adr->nmr),
		new TString("adrblk", adr->blk, sizeof adr->blk),
		new TString("adrvhd", adr->vhd, sizeof adr->vhd),
		new TString("adretj", adr->etj, sizeof adr->etj),
		new TString("adrapr", adr->apr, sizeof adr->apr),
		new TString("adrmsttxt", adr->msttxt, sizeof adr->msttxt),
		new TEisAlias("adrloc", &adr->loc),
		NULL
	);

	adr->loc = loc_v_nsm;

	// NPRPNESTA
	Add(
		new TDate("nprpnedta", &nprpnesta->dta, false),
		new TEisAlias("nprpnests", &nprpnesta->sts),
		NULL
	);
	// PNESTA
	Add(
		new TEisAlias("pnekcq", &pnesta->kcq),
		new TEisAlias("pnestpdvs", &pnesta->stpdvs),
		new TEisAlias("pnests", &pnesta->sts),
		NULL
	);
}

TGroup *PNE::Clone()
{
	return new PNE;
}

void PNE::Print(mstr &m)
{
	if (any(nmr))
		m.cpy(nmr);
	else if (sid)
	{
		m.clear();
		m.printf("(%d)", sid);
	}

	if (pnesta->kcq)
	{
		m.sep(" ");
		m.cat(EisCrime->Seek(pnesta->kcq));
	}
}

long find_pne_index(TXArray<PNE> *pnes, long pnesid)
{
	for (int i = 0; i < pnes->Count(); i++)
		if ((*pnes)[i]->sid == pnesid)
			return i;

	return INT_MAX;
}

PNE *find_pne(TXArray<PNE> *pnes, long pnesid)
{
	int i = find_pne_index(pnes, pnesid);
	return i == INT_MAX ? NULL : (*pnes)[i];
}

// ----- NKZPNE --------------------------------------------------------------
NKZPNE::NKZPNE()
	: TXMLGroup("NKZPNE")
{
	attribs.Add(
		new TXMLInt("nkzpneden", &den),
		new TXMLInt("nkzpnegdn", &gdn),
		new TXMLInt("nkzpnemsc", &msc),
		new TXMLMoney("nkzpnerzm", &rzm),
		new TXMLInt("nkzpnesdc", &sdc),
		new TXMLSid("nkzpnesid", &sid),
		new TXMLInt("nkzpnevid", &vid),
		NULL
	);

	Apply("nkzpnevid", XML_REQUIRED);
	Apply("nkzpnerzm", XML_NULLABLE);
	Apply("*", XML_INCLUDED | XML_EDITABLE);	// (no UI for sid)

	Add(
		new TLong("nkzpneden", &den),
		new TLong("nkzpnegdn", &gdn),
		new TLong("nkzpnemsc", &msc),
		new TMoney("nkzpnerzm", &rzm),
		new TLong("nkzpnesdc", &sdc),
		new TLong("nkzpnesid", &sid),
		new TEisAlias("nkzpnevid", &vid),
		NULL
	);
}

void NKZPNE::Print(mstr &m)
{
	m.cpy(EisVal->Seek(nmk_nkzvid, vid));
}

// ----- SCQ -----------------------------------------------------------------
SCQ::SCQ()
	: TXMLObject("SCQ")
{
	nkzpne = new TXARRAY(NKZPNE);
	objects.Add(nkzpne);

	attribs.Add(
		new TXMLDate("scqdta", &dta),
		new TXMLInt("scqosn", &osn),
		new TXMLInt("scqrlq", &rlq),
		new TXMLSid("scqsid", &sid),
		new TXMLInt("scqstr", &str),
		new TXMLString("scqstv", stv, sizeof stv),
		new TXMLInt("scqvid", &vid),
		NULL
	);
}

// ----- NPRFZLPNE -----------------------------------------------------------
NPRFZLPNE::NPRFZLPNE(const char *xmlName, TGroup *(*tClone)())
	: TEisGroup(xmlName ? xmlName : "NPRFZLPNE", "T_NPRFZLPNE", tClone ? tClone : NPRFZLPNE::Clone)
{
	scq = new SCQ;
	npl = new NPL(this);
	sbc = new SBC;
	vsl = new VSL(this);
	objects.Add(scq, npl, sbc, vsl, NULL);

	attribs.Add(
		new TXMLSid("fzlsid", &fzlsid, SID_FZL),
		new TXMLSid("pnesid", &pnesid, SID_PNE),
		new TXMLSid("fzlpnesid", &fzlpnesid, SID_NPRFZLPNE),
		NULL
	);

	Apply("*", XML_REQUIRED);

	// NPRFZLPNE
	Add(
		new TLong("fzlsid", &fzlsid),
		new TLong("pnesid", &pnesid),
		NULL
	);
	// SCQ
	Add(
		new TDate("scqdta", &scq->dta, false),
		new TEisAlias("scqosn", &scq->osn),
		new TEisAlias("scqrlq", &scq->rlq),
		new TLong("scqsid", &scq->sid),
		new TEisAlias("scqstr", &scq->str),
		new TString("scqstv", scq->stv, sizeof scq->stv),
		new TEisAlias("scqvid", &scq->vid),
		NULL
	);
	// SBC
	Add(
		new TEisAlias("sbcple", &sbc->ple),
		new TEisAlias("sbcrcd", &sbc->rcd),
		new TEisAlias("sbcspj", &sbc->spj),
		new TEisAlias("sbcetn", &sbc->etn),
		new TEisAlias("sbcobr", &sbc->obr),
		new TEisAlias("sbctrd", &sbc->trd),
		new TEisAlias("sbcznq", &sbc->znq),
		new TEisAlias("sbcrge", &sbc->rge),
		new TEisAlias("sbcgrj", &sbc->grj),
		new TLong("sbcsid", &sbc->sid),
		NULL
	);

	descs = Descs + TABLE_NPRFZLPNE;
}

TGroup *NPRFZLPNE::Clone()
{
	return new NPRFZLPNE;
}

void NPRFZLPNE::ExactCriteria(msql &m, const char *fields)
{
	NPRFZLPNE::FloatCriteria(m, fields);
	m.Add(this, "pnesid");
}

void NPRFZLPNE::FloatCriteria(msql &m, const char *fields)
{
	TEisGroup::FloatCriteria(m, fields);
	m.Add(this, "fzlsid");
}

long find_nprfzlpne_index(TXArray<NPRFZLPNE> *nprfzlpnes, long pnesid)
{
	for (int i = 0; i < nprfzlpnes->Count(); i++)
		if ((*nprfzlpnes)[i]->pnesid == pnesid)
			return i;

	return INT_MAX;
}

NPRFZLPNE *find_nprfzlpne(TXArray<NPRFZLPNE> *nprfzlpnes, long pnesid)
{
	int i = find_nprfzlpne_index(nprfzlpnes, pnesid);
	return i == INT_MAX ? NULL : (*nprfzlpnes)[i];
}

// ----- DLOOSN --------------------------------------------------------------
DLOOSN::DLOOSN()
	: TXMLGroup("DLOOSN")
{
	attribs.Add(
		new TXMLInt("dlogdn", &gdn),
		new TXMLInt("dloncnone", &ncnone),
		new TXMLString("dlonmr", nmr, sizeof nmr),
		new TXMLInt("dlopmt", &pmt),
		new TXMLSid("dlosid", &sid),
		new TXMLInt("dlosig", &sig),
		new TXMLInt("dlosprvid", &sprvid),
		new TXMLInt("dlostr", &str),
		new TXMLInt("dlovid", &vid),
		NULL
	);

	Apply("dlopmt", XML_REQUIRED | XML_NULLABLE);

	Add(
		new TLong("dlogdn", &gdn),
		new TEisAlias("dloncnone", &ncnone),
		new TXMLString("dlonmr", nmr, sizeof nmr),
		new TLong("dlopmt", &pmt),
		new TLong("dlosid", &sid),
		new TEisAlias("dlosig", &sig),
		new TEisAlias("dlosprvid", &sprvid),
		new TEisAlias("dlostr", &str),
		new TEisAlias("dlovid", &vid),
		NULL
	);
}

const char *DLOOSN::LoadConnect(const TConnect *connect)
{
	ConnectType->Seek(connect->connectType);

	if (!ConnectType->eispp)
		return "липсва ЕИСПП номер на основния вид дело";

	if (!connect->xferKind)
		return "липсва точен вид дело";

	XferKind->Seek(connect->xferKind);

	if (!XferKind->eispp)
		return "липсва ЕИСПП номер на точния вид дело";

	TSender sender;
	sender << connect;

	if (!sender.Get())
		return "липсващ адресант";

	if (!sender.eispp)
		return "липсва ЕИСПП номер на адресанта";

	gdn = connect->connectYear;
	strcpy(nmr, connect->connectNo);
	vid = ConnectType->eispp;
	str = sender.eispp;
	sig = XferKind->eispp;

	return NULL;
}

// ----- NFL -----------------------------------------------------------------
NFL::NFL(const char *xmlName, TGroup *(*tClone)())
	: TXMLGroup(xmlName ? xmlName : "NFL", NULL, tClone ? tClone : NFL::Clone), adr(new ADR)
{
	objects.Add(adr);

	attribs.Add(
		new TXMLInt("nfldrj", &drj),
		new TXMLString("nfleik", eik, sizeof eik),
		new TXMLInt("nflgrp", &grp),
		new TXMLInt("nfljrdstt", &jrdstt),
		new TXMLString("nflplnnme", plnnme, sizeof plnnme),
		new TXMLSid("nflsid", &sid, SID_NFL),
		new TXMLString("nflskrnme", skrnme, sizeof skrnme),
		new TXMLInt("nflvid", &vid),
		NULL
	);
}

TGroup *NFL::Clone()
{
	return new NFL;
}

// ----- sDLO ----------------------------------------------------------------
sDLO::sDLO(const char *xmlName, TGroup *(*tClone)())
	: TXMLGroup(xmlName ? xmlName : "sDLO", NULL, tClone)
{
	attribs.Add(
		new TXMLInt("dlogdn", &gdn),
		new TXMLInt("dloncnone", &ncnone),
		new TXMLString("dlonmr", nmr, sizeof nmr),
		new TXMLInt("dlopmt", &pmt),
		new TXMLSid("dlosid", &sid),
		new TXMLInt("dlosig", &sig),
		new TXMLInt("dlosprvid", &sprvid),
		new TXMLInt("dlostr", &str),
		new TXMLInt("dlovid", &vid),
		NULL
	);

	Apply("dlopmt", XML_REQUIRED | XML_NULLABLE);
}

TGroup *sDLO::Clone()
{
	return new DLO;
}

// ----- DLO -----------------------------------------------------------------
DLO::DLO()
	: sDLO("DLO", DLO::Clone)
{
	flags = XML_INCLUDED | XML_REQUIRED;

	dlosta = new DLOSTA;
	fzl = new TXARRAY(FZL);
	nfl = new TXARRAY(NFL);
	pne = new TXARRAY(PNE);
	nprfzlpne = new TXARRAY(NPRFZLPNE);
	dloosn = new TXARRAY(DLOOSN);
	objects.Add(dlosta, fzl, nfl, pne, nprfzlpne, dloosn, NULL);
}

TGroup *DLO::Clone()
{
	return new DLO;
}

void DLO::LoadConnects(TRCDKeyContainer *container)
{
	TGArray<TConnect> connects;

	load_connects(container, &connects);

	for (int i = 0; i < connects.Count(); i++)
	{
		DLOOSN *tDloosn = dloosn->NewObject();

		if (!tDloosn->LoadConnect(connects[i]))
			dloosn->Add(tDloosn);
		else
			delete tDloosn;
	}
}

// ----- NPR -----------------------------------------------------------------
NPR::NPR()
	: TNmrGroup("NPR", "T_NPR", NULL, "nprnmr", EISPP_NPR_LOCAL), fzlmin(0), minsid(0)
{
	flags = XML_REQUIRED;

	dlo = new DLO;
	objects.Add(dlo, NULL);

	attribs.Add(
		new TXMLInt("nprdrj", &drj),
		new TXMLDate("nprdta", &dta),
		new TXMLInt("nprndzprk", &ndzprk),
		new TXMLSid("nprsid", &sid),
		new TXMLInt("nprstr", &str),
		new TXMLString("npropitxt", opitxt, sizeof opitxt),
		NULL
	);

	// NPR
	Add(
		new TString("F_NPRNMR", nmr, SIZE_OF_EISPP_NMR),
		new TEisAlias("nprdrj", &drj),
		new TDate("nprdta", &dta, false),
		new TEisAlias("nprndzprk", &ndzprk),
		new TLong("nprsid", &sid),
		new TEisAlias("nprstr", &str),
		new TString("npropitxt", opitxt, sizeof opitxt),
		NULL
	);
	// DLO
	Add(
		new TLong("dlogdn", &dlo->gdn),
		new TEisAlias("dloncnone", &dlo->ncnone),
		new TString("dlonmr", dlo->nmr, sizeof dlo->nmr),
		new TLong("dlopmt", &dlo->pmt),
		new TLong("dlosid", &dlo->sid),
		new TEisAlias("dlosig", &dlo->sig),
		new TEisAlias("dlosprvid", &dlo->sprvid),
		new TEisAlias("dlostr", &dlo->str),
		new TEisAlias("dlovid", &dlo->vid),
		NULL
	);
	// DLOSTA
	Add(
		new TLong("dlostasid", &dlo->dlosta->sid),
		new TDate("dlodtasts", &dlo->dlosta->dtasts, false),
		new TEisAlias("dlosts", &dlo->dlosta->sts),
		NULL
	);

	sid = 1;
	descs = Descs + TABLE_NPR;
}

void NPR::ExactCriteria(msql &m, const char *fields)
{
	TEisGroup::FloatCriteria(m, fields);
}

void NPR::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(TGroup::Name, fields);
	m.Add(this, "nprnmr");
}

long NPR::Generate()
{
	long sid = FetchMinSid(minsid);
	return sid + 1;
}

long NPR::FetchMinSid(long absmin)
{
	for (int i = 0; i < dlo->fzl->Count(); i++)
		if ((*dlo->fzl)[i]->sid > absmin)
			absmin = (*dlo->fzl)[i]->sid;

	for (int i = 0; i < dlo->pne->Count(); i++)
		if ((*dlo->pne)[i]->sid > absmin)
			absmin = (*dlo->pne)[i]->sid;

	if (absmin >= SID_MAXIMUM)
		generator_limit(TGroup::Name);

	return absmin;
}

void NPR::InitGenerator()
{
	if (!minsid)
		minsid = FetchMinSid(SID_RESERVED);
}

void NPR::LoadSideWinds(TRCDKeyContainer *container, TSideWindArray *sideWinds)
{
	TCitizen citizen;
	long sid = SID_RESERVED;

	for (int i = 0; i < sideWinds->Count(); i++)
	{
		TSideWind *sideWind = (*sideWinds)[i];

		if (strchr(UCN_CITIZENS, sideWind->ucnType) && container->Defendant(sideWind->involvement))
		{
			strcpy(citizen.ucn, sideWind->ucn);
			citizen.ucnType = sideWind->ucnType;

			if (citizen.Get())
			{
				FZL fzl;

				fzl << this;
				fzl.LoadCitizen(&citizen);

				if (++sid > SID_MAXIMUM)
					generator_limit(TGroup::Name);

				fzl.sid = sid;
				fzl.Insert();
			}
		}
	}
}

// ----- SBE -----------------------------------------------------------------
SBE::SBE()
	: TXMLGroup("SBE"), dirty(false)
{
	Init();
}

SBE::SBE(long tVid)
	: TXMLGroup("SBE"), dirty(false)
{
	Init();
	ApplyDesc(tVid);
}

void SBE::ApplyDesc(long tVid)
{
	TGArray<TSbeDesc> descs;

	intvid = tVid;
	SbeDesc->vid = intvid;
	SbeDesc->LoadFloat(descs);

	if (!descs.Count())
		fatal("%d: no sbe desc", tVid);

	Apply(&descs);

	if (intvid > 0)
	{
		if (Includes("sbhvid") && (sbhnmk = FetchDesc(".sbhvid")) == 0)
			fatal("%d: missing .sbhvid", vid);

		if (npr->dlo->Includes("FZL"))
			npr->scqnmk = FetchDesc(".scqosn");

		if (npr->dlo->Includes("NPRFZLPNE"))
			npr->nprfzlnmk = FetchDesc(".nprfzlosn");
	}
}

long SBE::FetchDesc(const char *name)
{
	long value;
	TSbeDesc desc;
	TXMLInt fetch(name, &value);

	desc.vid = vid;
	strcpy(desc.name, name);

	if (desc.Try())
		fetch.Apply(&desc);

	return value;
}

void SBE::Init()
{
	sbh = new SBH;
	srk = new SRK;
	dvjdlo = new DVJDLO;
	npr = new NPR;
	sbe = new TXARRAY(SBE);
	objects.Add(sbh, srk, dvjdlo, npr, sbe, NULL);

	attribs.Add(
		new TXMLDate("sbedkpdta", &dkpdta),
		new TXMLString("sbedkpnmr", dkpnmr, sizeof dkpnmr),
		new TXMLInt("sbedkpstr", &dkpstr),
		new TXMLInt("sbedkpvid", &dkpvid),
		new TXMLSid("sbesid", &sid),
		new TXMLInt("sbesljstr", &sljstr),
		new TXMLInt("sbestpskt", &stpskt),
		new TXMLInt("sbevid", &vid),
		new TXMLString("elementType", elementType, sizeof elementType),
		NULL
	);

	Apply("elementType", XML_REQUIRED);

	// SBE
	Add(
		new TDate("sbedkpdta", &dkpdta, false),
		new TString("sbedkpnmr", dkpnmr, sizeof dkpnmr),
		new TEisAlias("sbedkpstr", &dkpstr),
		new TEisAlias("sbedkpvid", &dkpvid),
		new TLong("sbesid", &sid),
		new TEisAlias("sbesljstr", &sljstr),
		new TEisAlias("sbestpskt", &stpskt),
		new TEisAlias("sbevid", &vid),
		new TString("elementType", elementType, sizeof elementType),
		NULL
	);
	// SBH
	Add(
		new TLong("sbhsid", &sbh->sid),
		new TEisAlias("sbhstn", &sbh->stn),
		new TString("sbhtxt", sbh->txt, sizeof sbh->txt),
		new TEisAlias("sbhvid", &sbh->vid),
		NULL
	);
	// SRK
	Add(
		new TShort("srkden", &srk->den),
		new TDate("srkdta", &srk->dta, false),
		new TDate("srkdtadod", &srk->dtadod, false),
		new TShort("srkgdn", &srk->gdn),
		new TShort("srkmsc", &srk->msc),
		new TShort("srkobt", &srk->obt),
		new TShort("srksdc", &srk->sdc),
		new TLong("srksid", &srk->sid),
		new TEisAlias("srkvid", &srk->vid),
		NULL
	);
	// DVJDLO
	Add(
		new TLong("dvjdlosid", &dvjdlo->sid),
		new TEisAlias("dvjprc", &dvjdlo->prc),
		new TDate("dvjdta", &dvjdlo->dta, false),
		new TEisAlias("dvjstripr", &dvjdlo->stripr),
		new TEisAlias("dvjstrplc", &dvjdlo->strplc),
		new TEisAlias("dvjvid", &dvjdlo->vid),
		new TString("dvjdloopi", dvjdlo->opi, sizeof dvjdlo->opi),
		new TString("dvjdlodvn", dvjdlo->dvn, sizeof dvjdlo->dvn),
		NULL
	);
}

long SBE::Index(long vid, long sid)
{
	long nomer = sbe_etypenomer.Find(vid);
	// 2012:346 LPR: // % SID_MAXIMUM, we don't read sbezvk any more
	return sid ? (nomer << 16) + labs(sid) : nomer;
}

long SBE::Index()
{
	return Index(vid, EisVal->Contains(nmk_sbeednfzl, vid) || intvid == sbe_edit_fzl ? (*npr->dlo->fzl)[0]->sid :
		intvid == sbe_edit_pne_reg ? (*npr->dlo->pne)[0]->sid : 0);
}

void SBE::LoadFzlPneNpr()
{
	FZL fzl;
	PNE pne;
	NPRFZLPNE nprfzlpne;

	fzl.LoadFloat(npr->dlo->fzl, npr);
	pne.LoadFloat(npr->dlo->pne, npr);
	nprfzlpne.LoadFloat(npr->dlo->nprfzlpne, npr);
}

void SBE::Print(mstr &m)
{
	m.cpy(EisVal->Seek(nmk_sbevid, vid));

	if (EisVal->Contains(nmk_sbeednfzl, vid))
	{
		mstr w;

		m.cat(" - ");
		(*npr->dlo->fzl)[0]->Print(w);
		m.cat(w);
	}
}

void SBE::Write(TEisFile &x)
{
	if (vid && !strcmp(elementType, "sbezvk"))
	{
		long sid = x.sid;

		x.sid = -1;
		TXMLGroup::Write(x);
		x.sid = sid;
	}
	else
		TXMLGroup::Write(x);
}

// ----- VHD -----------------------------------------------------------------
VHD::VHD(SBE *tSbe)
	: TXMLRequiredObject("VHD"), sbe(tSbe)
{
	objects.Add(sbe);
	Apply("SBE", XML_INCLUDED);
}

// ----- KST -----------------------------------------------------------------
KST::KST(SBE *sbe)
	: TXMLRequiredObject("KST")
{
	attribs.Add(
		new TXMLString("armStrSid", armStrSid, sizeof armStrSid),
		new TXMLString("sesSid", sesSid, sizeof sesSid),
		new TXMLString("usrRab", usrRab, sizeof usrRab),
		new TXMLString("usrRej", usrRej, sizeof usrRej),
		new TXMLString("usrSid", usrSid, sizeof usrSid),
		new TXMLString("armSid", armSid, sizeof armSid),
		new TXMLInt("resSid", &resSid),
		new TXMLString("lgtSid", lgtSid, sizeof lgtSid),
		new TXMLString("sbeVid", sbeVid, sizeof sbeVid),
		new TXMLDate("sbeDta", &sbeDta),
		new TXMLString("okaSid", okaSid, sizeof okaSid),	// 2012:333 LPR: uncomented
		NULL
	);

	Apply("*", XML_REQUIRED | XML_NULLABLE);

	strcpy(usrRab, EISPP_KST_USRRAB);
	strcpy(usrRej, EISPP_KST_USRREJ);
	strcpy(usrSid, EISPP_KST_USRSID);
	strcpy(armSid, EISPP_KST_ARMSID);
	// 2012:333
	strcpy(okaSid, EISPP_KST_OKASID);
	strcpy(armStrSid, EISPP_KST_ARMSTRSID);
	strcpy(sesSid, EISPP_KST_SESSID);
	strcpy(lgtSid, EISPP_KST_LGTSID);

	strcpy(sbeVid, EISPP_KST_SBEVID);
	sbeDta = Today;

	if (sbe)
	{
		// 2012:333
		if (sbe->vid)
			itoa(sbe->vid, sbeVid, 10);

		sbeDta = sbe->dkpdta.Empty() ? Today : sbe->dkpdta;	// 2012:333 FIX: Today if !dkpdta
	}
}

// ----- SubContext ----------------------------------------------------------
SubContext::SubContext()
	: TXMLRequiredObject("SubContext")
{
	attribs.Add(
		new TXMLString("delovodenNumber", delovodenNumber, sizeof delovodenNumber),
		new TXMLString("ConfidenceLevel", ConfidenceLevel, sizeof ConfidenceLevel),
		new TXMLInt("creatorSID", &creatorSID),
		new TXMLString("destinSID", destinSID, sizeof destinSID),
		new TXMLString("dispTemplate", dispTemplate, sizeof dispTemplate),
		new TXMLString("fileType", fileType, sizeof fileType),
		new TXMLString("toDate", toDate, sizeof toDate),
		new TXMLString("fromDate", fromDate, sizeof fromDate),
		NULL
	);

	Apply("*", XML_REQUIRED);

	strcpy(delovodenNumber, "DEFAULT_NUMBER");
	strcpy(ConfidenceLevel, "high");
	{
		Court->Seek(COURT_CODE);
		creatorSID = Court->eispp;
	}
	strcpy(destinSID, "str_mp_eispp");
	strcpy(dispTemplate, "type_table");
	strcpy(fileType, "spr_rzt_html");
	sprintf(toDate, "%04ld-%02ld-%02ldT23:59:59", Today.year, Today.month, Today.day);
	strcpy(fromDate, "1900-01-01T00:00:00.000");
};

// ----- Argument ------------------------------------------------------------
Argument::Argument(const char *nprnmr)
	: TXMLRequiredObject("Argument")
{
	attribs.Add(
		new TXMLInt("IDArg", &IDArg),
		new TXMLString("value", value, sizeof value),
		NULL
	);

	Apply("*", XML_REQUIRED);
	IDArg = 357;
	strcpy(value, nprnmr);
}

// ----- TIPVHD --------------------------------------------------------------
TIPVHD::TIPVHD(const char *nprnmr)
	: TXMLRequiredObject("TIPVHD"), argument(new Argument(nprnmr))
{
	objects.Add(argument);
	attribs.Add(new TXMLInt("IDSpravka", &IDSpravka));
	Apply("*", XML_REQUIRED);
	IDSpravka = 12536;
}

// ----- ZQKSPRVHD -----------------------------------------------------------
ZQKSPRVHD::ZQKSPRVHD(const char *nprnmr)
	: TXMLRequiredObject("ZQKSPRVHD"), subcontext(new SubContext), tipvhd(new TIPVHD(nprnmr))
{
	objects.Add(subcontext, tipvhd, NULL);

	attribs.Add(
		new TXMLString("TypeSpravka", TypeSpravka, sizeof TypeSpravka),
		new TXMLString("ZaqvkaName", ZaqvkaName, sizeof ZaqvkaName),
		new TXMLString("subType", subType, sizeof subType),
		new TXMLInt("IDZaqvka", &IDZaqvka),
		new TXMLString("SPRDescription", SPRDescription, sizeof SPRDescription),
		NULL);

	Apply("*", XML_REQUIRED);
	strcpy(TypeSpravka, "spr_tipova");
	strcpy(ZaqvkaName, "ЕИСПП_СС_18");
	strcpy(subType, "spr_interakt");
	IDZaqvka = 12536;
	strcpy(SPRDescription, "ЕИСПП_СС_18");
}

// ----- sFZL ----------------------------------------------------------------
sFZL::sFZL()
	: FZL("sFZL", sFZL::Clone)
{
}

TGroup *sFZL::Clone()
{
	return new sFZL;
}

// ----- sNFL ----------------------------------------------------------------
sNFL::sNFL()
	: NFL("sNFL", sNFL::Clone)
{
}

TGroup *sNFL::Clone()
{
	return new sNFL;
}

// ----- PNESPF --------------------------------------------------------------
PNESPF::PNESPF()
	: TXMLGroup("PNESPF")
{
	attribs.Add(new TXMLString("pnespfopi", opi, sizeof opi));
}

// ----- sPNE ----------------------------------------------------------------
sPNE::sPNE()
	: PNE("sPNE", sPNE::Clone)
{
	pnespf = new TXARRAY(PNESPF);

	objects.Add(new TXMLUnknownArray("PNEPSL"), pnespf, new TXMLUnknownArray("ADR"), nprpnesta,
		new TXMLUnknownArray("PSG"), pnesta, new TXMLUnknownArray("PNENFL"), NULL);

	adr->tip = adr_mestoprestaplenie;
}

TGroup *sPNE::Clone()
{
	return new sPNE;
}

sPNE::~sPNE()
{
	delete adr;
}

// ----- sNPRFZLPNE ----------------------------------------------------------
sNPRFZLPNE::sNPRFZLPNE()
	: NPRFZLPNE("sNPRFZLPNE", sNPRFZLPNE::Clone)
{
}

TGroup *sNPRFZLPNE::Clone()
{
	return new sNPRFZLPNE;
}

// ----- sNPRAKTSTS ----------------------------------------------------------
sNPRAKTSTS::sNPRAKTSTS()
	: TNmrGroup("sNPRAKTSTS", "T_SNPR", NULL, "nprnmr", EISPP_NPR_LOCAL)
{
	fzl = new TXARRAY(sFZL);
	nfl = new TXARRAY(sNFL);
	pne = new TXARRAY(sPNE);
	nprfzlpne = new TXARRAY(sNPRFZLPNE);
	objects.Add(new TXMLUnknownObject("sNPRVID"), new TXMLUnknownObject("sNPRSTA"), new TXMLUnknownObject("sNPRSTE"),
		new TXMLUnknownArray("sDLO"), fzl, nfl, pne, nprfzlpne, NULL);

	attribs.Add(new TXMLInt("nprdrj", &drj));

	descs = Descs + TABLE_SNPR;
}

void sNPRAKTSTS::ExactCriteria(msql &m, const char *fields)
{
	TEisGroup::FloatCriteria(m, fields);
}

// ----- RZTOPRSPR -----------------------------------------------------------
RZTOPRSPR::RZTOPRSPR()
	: TXMLRequiredObject("RZTOPRSPR"), npr(new sNPRAKTSTS)
{
	objects.Add(npr);
}

// ----- RZT -----------------------------------------------------------------
RZT::RZT()
	: TXMLRequiredObject("RZT"), rztoprspr(new RZTOPRSPR)
{
	objects.Add(rztoprspr);
}

// ----- DATA ----------------------------------------------------------------
DATA::DATA(SBE *sbe)
	: TXMLRequiredObject("DATA"), vhd(new VHD(sbe)), kst(new KST(sbe))
{
	objects.Add(vhd, kst, NULL);
}

DATA::DATA(const char *nprnmr)
	: TXMLRequiredObject("DATA"), zqksprvhd(new ZQKSPRVHD(nprnmr)), kst(new KST(NULL))
{
	objects.Add(zqksprvhd, kst, NULL);
}

DATA::DATA()
	: TXMLRequiredObject("DATA"), rzt(new RZT)
{
	objects.Add(rzt);
}

// ----- Property ------------------------------------------------------------
Property::Property()
	: TXMLGroup("Property")
{
	Init(NULL);
}

Property::Property(const char *tName, const char *s)
	: TXMLGroup("Property")
{
	Init(tName);
	strcpy(value, s);
}

Property::Property(const char *tName, long l)
	: TXMLGroup("Property")
{
	Init(tName);
	itoa(l, value, 10);
}

void Property::Init(const char *tName)
{
	attribs.Add(
		new TXMLString("name", name, sizeof name),
		new TXMLString("value", value, sizeof value),
		NULL
	);

	if (tName)
		strcpy(name, tName);

	flags = XML_REQUIRED;
	Apply("*", XML_REQUIRED);
}

// ----- Properties ----------------------------------------------------------
Properties::Properties(long pkt_tip)
	: TXMLRequiredObject("Properties")
{
	property = new TXARRAY(Property);
	objects.Add(property);
	Apply("Property", XML_REQUIRED);

	property->Add(new Property("vzlizp", EISPP_VZLIZP));
	property->Add(new Property("vzlplc", EISPP_VZLPLC));
	property->Add(new Property("vid_saobstenie", pkt_tip));
	property->Add(new Property("structura_izp", EISPP_STRUCTURA_IZP));
	property->Add(new Property("structura_plc", EISPP_STRUCTURA_PLC));
	// lgt_sid: standart 12 p4.4: placed by eispp; send 0?..
	// 2012:341 LPR: Dextro request
	Property *correlation_id = new Property("correlation_id", "");
	correlation_id->Apply("value", XML_NULLABLE);
	property->Add(correlation_id);
}

// ----- EISPAKET ------------------------------------------------------------
EISPAKET::EISPAKET(SBE *sbe)
	: TXMLRequiredObject("EISPPPAKET"), data(new DATA(sbe)), properties(new Properties(pkt_tip_vhd_data))
{
	objects.Add(properties, data, NULL);
}

bool EISPAKET::Check(TRCDKeyContainer *container)
{
	SBE *sbe = data->vhd->sbe;
	char name[SIZE_OF_PATH];
	TEisPending pending;

	electric_name(container, EISPP_OUTPATH, name, TYPE_SBE_DESC + sbe->Index());
	*strrchr(name, '\\') = '_';
	strcpy(pending.fileName, basename(name));

	while (exist(name) && pending.Try())
	{
		mstr m(pending.requestId == -1 ? "В момента се изпращат данни" : "Изчаква се отговор");

		m.printf(" за \"%s\" по %s %ld/%ld", EisVal->Seek(nmk_sbevid, sbe->vid), Type->Seek(container->key.type),
			container->key.no, container->key.year);

		if (strchr(KIND_SESSIONS_ENDOCS, container->kind))
			m.printf(", %s от %D", Kind->Seek(container->kind), &container->date);

		if (!ask("%s.\n\nЖелаете ли да опитате отново?", str(m)))
		{
			m.clear();
			m.printf("%s: rj=%d", pending.fileName, pending.requestId);
			log(LOG_EISPP, 0, container, str(m));
			return false;
		}
	}

	pending.Delete(true);
	return true;
}

void EISPAKET::Insert(TRCDKeyContainer *container)
{
	if (Check(container))
	{
		SBE *sbe = data->vhd->sbe;
		SBE *zvk = ReadEF(container, sbe->Index(), true);

		if (zvk)
		{
			strcpy(zvk->elementType, "sbezvk");
			data->vhd->objects.Insert(zvk, 0);
			data->kst->resSid = mnu_reg_korsbe;
		}
		else
		{
			TSbeDesc desc;

			// 2013:058
			if (sbe->vid == sbe_otkpmnchjpsd && sbe->npr->dlo->sprvid == 1025)
			{
				sbe->npr->dlo->sprvid = 0;
				sbe->npr->dlo->Apply(&TSbeDesc("dlosprvid", XML_REQUIRED, "14120"));
			}
			// 2012:345; 2012:003 FIX: before resSid
			else
			{
				desc.vid = sbe->vid;
				strcpy(desc.name, ".dlosprvid");

				if (desc.Try())
				{
					sbe->npr->dlo->sprvid = 0;
					strcpy(desc.name, "dlosprvid");
					sbe->npr->dlo->Apply(&desc);
				}
			}

			desc.vid = sbe->vid;
			strcpy(desc.name, ".resSid");

			if (desc.Try())
			{
				strcpy(desc.name, "resSid");
				data->kst->Apply(&desc);
			}
			else
			{
				const TSpr2Mnu *spr2mnu;

				for (spr2mnu = spr2mnus; spr2mnu->spr; spr2mnu++)
					if (sbe->npr->dlo->sprvid == spr2mnu->spr)
						break;

				data->kst->resSid = spr2mnu->mnu;
			}

			if (!data->kst->resSid)
			{
				xml_error_now(container, "sbereg", "vid = %ld, spr = %ld, res=??", sbe->vid, sbe->npr->dlo->sprvid);
				return;
			}
		}

		strcpy(sbe->elementType, "sbereg");

		// 2013:035 LPR: we read ADR from T_PNE now
		for (int i = 0; i < sbe->npr->dlo->pne->Count(); i++)
		{
			ADR *adr = (*sbe->npr->dlo->pne)[i]->adr;

			if (adr->Includes("adrloc"))
			{
				if (adr->drj)
					adr->flags |= XML_INCLUDED;
				else
					adr->flags &= ~XML_INCLUDED;

				if (!adr->loc)
					adr->loc = loc_v_nsm;
			}
		}

		mstr m;

		if (!IsValid(m))
		{
			xml_error_now(container, "Запис", "%s", str(m));
			return;
		}

		Write(container);
	}
}

SBE *EISPAKET::ReadEF(const char *name, TRCDKeyContainer *container, bool sbezvk)
{
	try
	{
		TEisFile x(0);
		SBE *sbe1 = new SBE(sbe_edit_delete);
		SBE *sbe2 = new SBE(sbe_edit_delete);
		EISPAKET eispaket(sbe1);
		TIArray<TXMLStruct> &objects = eispaket.data->vhd->objects;

		sbe1->flags |= XML_REQUIRED;
		sbe2->flags |= XML_INCLUDED;
		objects.Add(sbe2);

		if (sbezvk)		// 2013:050 FIX: korpne lacks otd
		{
			TIArray<TXMLStruct> &objects = sbe1->npr->dlo->objects;

			for (int i = 0; i < objects.Count(); i++)
			{
				if (objects[i]->Name == "PNE")
				{
					objects.Remove(i);
					break;
				}
			}
		}

		char *const text = x.ReadAll(name);
		TCharPtr CP(text);

		eispaket.properties->objects.Flush();
		eispaket.data->kst->attribs.Flush();
		eispaket.ReadStruct(text);

		if (sbezvk)		// 2013:053 FIX: pne must exist
		{
			sbe1->npr->dlo->pne = new TXARRAY(PNE);
			sbe1->npr->dlo->objects.Add(sbe1->npr->dlo->pne);
		}

		if (!strcmp(sbe1->elementType, "sbezvk"))
		{
			objects.Remove(0);
			sbe1 = sbe2;
		}

		if (sbe1->vid && !strcmp(sbe1->elementType, "sbereg"))
		{
			objects.Detach(0);

			if (!sbezvk)
			{
				sbe1->ApplyDesc(eispp_get_sbevid(name));

				// 2013:008 from load_
				DLO *dlo = sbe1->npr->dlo;

				for (int i = 0; i < dlo->fzl->Count(); i++)
					(*dlo->fzl)[i]->sid *= -1;

				for (int i = 0; i < dlo->pne->Count(); i++)
					(*dlo->pne)[i]->sid *= -1;

				for (int i = 0; i < dlo->nprfzlpne->Count(); i++)
				{
					(*dlo->nprfzlpne)[i]->fzlsid *= -1;
					(*dlo->nprfzlpne)[i]->pnesid *= -1;
				}
			}

			return sbe1;
		}
	}
	catch (TXMLError *x)
	{
		x->Handle(container);
		delete x;
	}

	return NULL;
}

SBE *EISPAKET::ReadEF(TRCDKeyContainer *container, long index, bool sbezvk)
{
	char name[SIZE_OF_PATH];
	electric_name(container, EISPP_XMLPATH, name, TYPE_SBE_DESC + index);
	return exist(name) ? ReadEF(name, container, sbezvk) : NULL;
}

void EISPAKET::Write(TRCDKeyContainer *container)
{
	SBE *sbe = data->vhd->sbe;

	try
	{
		TEisFile x(sbe->npr->Generate());
		char name[SIZE_OF_PATH];
		char text[SIZE_OF_LOG_TEXT];	// 2012:353 +related

		electric_name(container, EISPP_OUTPATH, name, TYPE_SBE_DESC + sbe->Index());
		*strrchr(name, '\\') = '_';

		x.OpenWrite(name);
		Write(x);
		x.Close();

		sprintf(text, "%s за изпращане", strrchr(name, '\\') + 1);
		log(LOG_EISPP, 0, container, text);
	}
	catch (TXMLError *x)
	{
		x->Handle(container);
		delete x;
	}
}

void EISPAKET::Update(TRCDKeyContainer *container, SBE *zvk, long resSid)
{
	if (Check(container))
	{
		zvk->flags |= XML_INCLUDED;
		zvk->npr->sid = 0;
		strcpy(zvk->elementType, "sbezvk");
		data->vhd->objects.Insert(zvk, 0);
		strcpy(data->vhd->sbe->elementType, "sbereg");
		data->kst->resSid = resSid;
		Write(container);
	}
}

void EISPAKET::Delete(TRCDKeyContainer *container, SBE *zvk)
{
	EISPAKET eispaket(zvk);

	eispaket.data->kst->sbeDta = zvk->dkpdta;
	itoa(zvk->vid, eispaket.data->kst->sbeVid, 10);
	strcpy(zvk->elementType, "sbezvk");
	eispaket.data->kst->resSid = mnu_reg_itvsbe;
	eispaket.Write(container);
}

void EISPAKET::Delete(TRCDKeyContainer *container, long index)
{
	SBE *zvk = ReadEF(container, index, true);

	if (zvk)
		Delete(container, zvk);
}

// ----- TEisQuery -----------------------------------------------------------
TEisQuery::TEisQuery(TWindow *tParent, const char *tNprnmr, Properties *tProperties, DATA *tData,
	TOriginContainer *tContainer)
	: TXMLRequiredObject("EISPPPAKET"), parent(tParent), nprnmr(tNprnmr), properties(tProperties), data(tData),
		container(tContainer)
{
	objects.Add(properties);
	objects.Add(data);
}

sign TEisQuery::Execute()
{
	mstr m;

	build_fn(EISPP_OUTPATH, base, outName);
	m.cpy(outName);
	mbk_insert(&m, "\\invalid", m.rindex('\\'), 8);
	strcpy(errName, str(m));

	bool hasQuery = exist(xmlName);
	char *text = NULL;

	if (hasQuery && !ask("Вече има извършена справка за НП %s. Желаете ли да я използвате?", nprnmr))
	{
		if (unlink(xmlName))
			return error("%s: грешка %d при изтриване на XML файл.", xmlName, errno);

		hasQuery = false;
	}

	try
	{
		char tmpName[SIZE_OF_PATH];
		const char *queryName;

		if (hasQuery)
			queryName = xmlName;
		else
		{
			TEisFile x(0);
			char text[SIZE_OF_LOG_TEXT];

			if (!Check())
				return false;

			Write(x);
			sprintf(text, "справка за %s", nprnmr);
			log(LOG_EISPP, 0, text);

			queryName = tmpName;
			build_fn(tempdir, basename(xmlName), tmpName);
			hasQuery = eispp_wait(parent, nprnmr, xmlName, errName, tmpName);
		}

		if (hasQuery)
		{
			TEisFile x(0);

			// 2015:141 LPR: the 7evens in OS Varna seem misconfigured...
			// 2016:004 LPR: using CopyFile in eispp_wait
			//Sleep(COURT_CODE == COURT_VAROS && Win7OrLater ? 1000 : 100);
			Reset();
			text = x.ReadAll(queryName);
			ReadStruct(text);

			delete[] text;
			return true;
		}
	}
	catch (TXMLError2 *x)
	{
		x->Handle(container);
		delete x;
		return -1;
	}
	catch (TXMLError *x)
	{
		x->Handle(container);
		delete x;
	}

	delete[] text;
	return false;
}

// ----- EISTSAKT ------------------------------------------------------------
EISTSAKT::EISTSAKT(TWindow *parent, TOriginContainer *container, const char *nprnmr)
	: TEisQuery(parent, nprnmr, new Properties(0), new DATA, container)
{
}

sign EISTSAKT::Execute()
{
	char temp[SIZE_OF_PATH];

	sprintf(base, "q%s.xml", nprnmr);
	map_eispp(base, 1, 12);

	build_fn(EISPP_XMLPATH, "queries", temp);
	build_fn(temp, base, xmlName);

	return TEisQuery::Execute();
}

#define fzl OFFICE_GROUP_CAST(FZL)

static void fzl_name(mstr &m, TGroup *group)
{
	if (any(fzl->egn))
		m.printf("ЕГН %s ", fzl->egn);

	if (any(fzl->lnc))
	{
		m.sep("/ ");
		m.printf("ЛНЧ %s ", fzl->lnc);
	}

	if (any(fzl->imecyr))
		m.cat(fzl->imecyr);
	else if (any(fzl->imelat))
		m.cat(fzl->imelat);
	else
		m.printf("%s %s %s", fzl->ime, fzl->prz, fzl->fma);
}

#undef fzl

static void cleanup_fzl_pne_npr(TXMLArray *fzls, TXMLArray *pnes, TXMLArray *nprfzlpnes, long fzlmin, const CDate &sbedkpdta)
{
	for (int i = 0; i < fzls->Count(); )
	{
		FZL *fzl = (FZL *) (*fzls)[i];

		if (fzl->sid < fzlmin)
			fzls->Remove(i);
		else
			i++;
	}

	for (int i = 0; i < nprfzlpnes->Count(); )
	{
		NPRFZLPNE *nprfzlpne = (NPRFZLPNE *) (*nprfzlpnes)[i];

		int n;
		// 2013:259 FIX: for imported data
		for (n = 0; n < fzls->Count(); n++)
			if (((FZL *) (*fzls)[n])->sid == nprfzlpne->fzlsid)
				break;

		if (nprfzlpne->fzlsid < fzlmin || n == fzls->Count())
			nprfzlpnes->Remove(i);
		else
		{
			// 2012:353 LPR: for the only sbe which "requires" scqdta
			if ((nprfzlpne->FindAttrib("scqdta")->flags & XML_REQUIRED) && nprfzlpne->scq->dta.Empty())
				nprfzlpne->scq->dta = sbedkpdta;
			i++;
		}
	}

	for (int i = 0; i < pnes->Count(); )
	{
		int n;
		PNE *pne = (PNE *) (*pnes)[i];

		for (n = 0; n < nprfzlpnes->Count(); n++)
			if (((NPRFZLPNE *) (*nprfzlpnes)[n])->pnesid == pne->sid)
				break;

		if (n == nprfzlpnes->Count())
			pnes->Remove(i);
		else
			i++;
	}
}

#include "FzlListBox.h"
#include "FzlListBox.cpp"

#include "EisppSideDialog.h"
#include "EisppSideDialog.cpp"

bool EISTSAKT::Import()
{
	sNPRAKTSTS *npr = data->rzt->rztoprspr->npr;
	long sid = SID_RESERVED;

	for (int i = 0; i < npr->fzl->Count();)
	{
		sFZL *fzl = (sFZL *) (*npr->fzl)[i];

		// 2013:259 FIX: remove egn_neizvestno
		// 2016:106 FIX: allow egn_neizvestno in eis
		if (fzl->nprfzlsta->kcv == kcv_ne_e_svarzan_s_np)
			npr->fzl->Remove(i);
		else
			i++;
	}

	if (TEisppSideDialog(parent, npr).Execute() != IDOK)
		return false;	// ignore -> clear request nprnmr

	npr->Insert();

	mstr involvements;
	container->Involvements(involvements);

	for (int i = 0; i < npr->fzl->Count(); i++)
	{
		sFZL *fzl = (*npr->fzl)[i];
		TCitizen citizen;

		sid++;

		for (int i = 0; i < npr->nprfzlpne->Count(); i++)
		{
			sNPRFZLPNE *nprfzlpne = (*npr->nprfzlpne)[i];

			if (nprfzlpne->fzlsid == fzl->sid)
				nprfzlpne->fzlsid = sid;
		}

		// 2016:106 IRQ: don't save egn_neizvestno in CAC
		if (strcmp(fzl->egn, egn_neizvestno) && fzl->SaveCitizen(&citizen))
		{
			TSideWind sideWind;

			sideWind << container;
			sideWind << citizen;

		#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
			char lawKind = container->kind;

			if (container->key.type == TYPE_REQUEST)
				lawKind = ((const TRequest *) container)->lawKind;
		#endif  // AREAL || MILITARY || REGIONAL

			switch (fzl->nprfzlsta->kcv)
			{
				case kcv_obvinjaem : sideWind.involvement = INVOLVEMENT_ACCUSING; break;
				case kcv_podsadim :
				{
				#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
					if (lawKind == KIND_1ST_ADMIN_PUNISHMENT_LAW)
						sideWind.involvement = INVOLVEMENT_ACCUSING;
					else
				#endif  // AREAL || MILITARY
				#if COURT_TYPE == COURT_REGIONAL
					if (lawKind == KIND_ADMIN_PUNISHMENT_LAW)
						sideWind.involvement = INVOLVEMENT_ACCUSING;
					else
				#endif  // REGIONAL
				#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
					if (lawKind == KIND_PRIVATE_PUNISHMENT_LAW)
						sideWind.involvement = INVOLVEMENT_JUDGED;
					else
				#endif  // AREAL || MILITARY || REGIONAL
					{
						sideWind.involvement = INVOLVEMENT_JUDGING;
					}
					break;
				}
				default : sideWind.involvement = INVOLVEMENT_OTHER;
			}

			if (!involvements.chr(sideWind.involvement))
				sideWind.involvement = INVOLVEMENT_OTHER;

			if (!sideWind.Try())
				sideWind.Insert();
		}

		fzl->sid = sid;
		fzl->key = npr->key;
		fzl->Insert();
	}

	cleanup_fzl_pne_npr(npr->fzl, npr->pne, npr->nprfzlpne, -1, Today);

	for (int i = 0; i < npr->pne->Count(); i++)
	{
		sPNE *pne = (*npr->pne)[i];

		++sid;

		for (int i = 0; i < npr->nprfzlpne->Count(); i++)
		{
			sNPRFZLPNE *nprfzlpne = (*npr->nprfzlpne)[i];

			if (nprfzlpne->pnesid == pne->sid)
				nprfzlpne->pnesid = sid;
		}

		pne->sid = sid;
		pne->key = npr->key;
		pne->Insert();
	}

	for (int i = 0; i < npr->nprfzlpne->Count(); i++)
	{
		sNPRFZLPNE *nprfzlpne = (*npr->nprfzlpne)[i];

		nprfzlpne->key = npr->key;
		nprfzlpne->scq->str = COURT_EISPP;
		nprfzlpne->Insert();
	}

	return true;
}

static constant execTSAKTSTS1 =
	"<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" " //
	"xmlns:eis=\"http://eispp.bg/EISPP_Communication/\">\n"
	"	<soap:Header/>\n"
	"	<soap:Body>\n"
	"		<eis:EISPPPAKET>\n"
	"			<eis:Properties>\n"
	"				<eis:Property name=\"vzlplc\" value=\"%s\"/>\n"
	"				<eis:Property name=\"vzlizp\" value=\"%s\"/>\n"
	"				<eis:Property name=\"structura_izp\" value=\"%ld\"/>\n"
	"				<eis:Property name=\"structura_plc\" value=\"%ld\"/>\n"
	"			</eis:Properties>\n"
	"			<eis:DATA>\n"
	"				<eis:ZQKOPRSPR>\n"
	"					<eis:ZQKAKTSTS NPRNMR=\"";
static constant execTSAKTSTS2 =
										"\"/>\n"
	"				</eis:ZQKOPRSPR>\n"
	"				<eis:KST armStrSid=\"-1\" sesSid=\"-1\" usrRab=\"%s\" usrRej=\"%s\" usrSid=\"%s\" "
					"resSid=\"%d\" okaSid=\"-1\" sbeDta=\"%ld-%02d-%02d\" armSid=\"%s\" lgtSid=\"-1\" "
					"sbeVid=\"-1\"/>\n"
	"			</eis:DATA>\n"
	"		</eis:EISPPPAKET>\n"
	"	</soap:Body>\n"
	"</soap:Envelope>\n";

void EISTSAKT::Write(TEisFile &x)
{
	FILE *f = x.OpenQuery(outName);

	fprintf(f, execTSAKTSTS1, EISPP_VZLPLC, EISPP_VZLIZP, EISPP_STRUCTURA_IZP, EISPP_STRUCTURA_PLC);
	x.Put(nprnmr);
	fprintf(f, execTSAKTSTS2, EISPP_KST_USRRAB, EISPP_EIQ_USRREJ, EISPP_EIQ_USRSID, mnu_reg_oneprp, Today.year, Today.month,
		Today.day, EISPP_KST_ARMSID);
	x.Close();
}
