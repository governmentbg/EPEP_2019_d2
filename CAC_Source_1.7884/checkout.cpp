#include "cac.h"

// ----- TCheckAddr ----------------------------------------------------------
class TCheckAddr : public TAddress
{
public:
	TCheckAddr() { }

	bool Empty() const { return !street && (!uclp || uclp == COURT_UCLP) && !any(adrNo); }
	bool Similar(const TCheckAddr &that) const;
	void PrintF(THTMLFile &h) const;

	bool operator!=(const TCheckAddr &that) const;
};

bool TCheckAddr::Similar(const TCheckAddr &that) const
{
	if (uclp != that.uclp)
		return false;

	if (street && that.street && street != that.street)
		return false;

	return !any(adrNo) || !any(that.adrNo) || !strcmp(adrNo, that.adrNo);
}

void TCheckAddr::PrintF(THTMLFile &h) const
{
	UCLP->uclp = uclp;
	Street->uclp = uclp;
	Street->street = street;
	h.printf(" | %-25s %-30s %-7s", UCLP->Try() ? UCLP->name : "", Street->Try() ? Street->name : "", adrNo);
}

bool TCheckAddr::operator!=(const TCheckAddr &that) const
{
	return uclp != that.uclp || street != that.street || strcmp(adrNo, that.adrNo);
}

// ----- TCheckAddress -------------------------------------------------------
class TCheckAddress : public TGroup
{
public:
	TCheckAddress(const char *name, TGroup *(*const tClone)());

	char ucn[SIZE_OF_UCN];
	char ucnType;

	TGArray<TCheckAddr> addresses;

	void LoadAddrs();
	bool Similar(const TCheckAddress &that) const;
	void PrintF(THTMLFile &h) const;

protected:
	bool Similar(const TCheckAddr *that) const;
};

TCheckAddress::TCheckAddress(const char *name, TGroup *(*const tClone)())
	: TGroup(name, tClone)
{
	Add(
		new TString("F_UCN", ucn, sizeof ucn),
		new TCharAlias("F_UCN_TYPE", &ucnType),
		NULL
	);
}

void TCheckAddress::LoadAddrs()
{
	TCheckAddr *address = new TCheckAddr;
	msql m;

	*address << this;
	address->FloatCriteria(m, "F_UCLP, F_STREET, F_ADR_NO");

	TQuery q(str(m));
	while (q.Read())
	{
		*address << q;
		addresses.Add(address);
		address = new TCheckAddr;
	}

	delete address;
}

bool TCheckAddress::Similar(const TCheckAddress &that) const
{
	for (int i = 0; i < addresses.Count(); i++)
		if (that.Similar(addresses[i]))
			return true;

	return false;
}

void TCheckAddress::PrintF(THTMLFile &h) const
{
	for (int i = 0; i < addresses.Count(); i++)
		addresses[i]->PrintF(h);
	h.putc('\n');
}

bool TCheckAddress::Similar(const TCheckAddr *that) const
{
	for (int i = 0; i < addresses.Count(); i++)
		if (addresses[i]->Similar(*that))
			return true;

	return false;
}

// ----- TCheckCitizen -------------------------------------------------------
class TCheckCitizen : public TCheckAddress
{
public:
	TCheckCitizen();
	static TGroup *Clone();

	char name[SIZE_OF_CITIZEN_NAME];
	char reName[SIZE_OF_CITIZEN_RENAME];
	char family[SIZE_OF_CITIZEN_FAMILY];

	bool Similar(const TCheckCitizen &that) const;
	void PrintF(THTMLFile &h) const;
};

TCheckCitizen::TCheckCitizen()
	: TCheckAddress("T_CITIZEN", TCheckCitizen::Clone)
{
	Add(
		new TString("F_NAME", name, sizeof name),
		new TString("F_RENAME", reName, sizeof reName),
		new TString("F_FAMILY", family, sizeof family),
		NULL
	);
}

TGroup *TCheckCitizen::Clone()
{
	return new TCheckCitizen;
}

bool TCheckCitizen::Similar(const TCheckCitizen &that) const
{
	if (!TCheckAddress::Similar(that))
		return false;

	return !any(reName) || !any(that.reName) || !strcmp(reName, that.reName);
}

void TCheckCitizen::PrintF(THTMLFile &h) const
{
	h.printf("%3s %10s %-14s %-15s %-22s", UCNType->Seek(ucnType), ucn, name, reName, family);
	TCheckAddress::PrintF(h);
}

static void check_citizens(THTMLFile &h, TGArray<TCheckCitizen> &array)
{
	for (int i = 0; i < array.Count() - 1; i++)
	{
		bool printed = false;
		int n = i + 1;

		while (n < array.Count())
		{
			if (array[i]->Similar(*array[n]))
			{
				if (!printed)
				{
					array[i]->PrintF(h);
					printed = true;
				}
				array[n]->PrintF(h);
				array.Remove(n);
			}
			else
				n++;
		}

		if (printed)
			h.putc('\n');
	}
}

void checkout_citizens(TWindow *parent)
{
	CTime start = Now();
	THTMLFile h;
	h.OpenTemp();

	TWaitWindow wait(parent, WAIT);
	msql m;
	TCheckCitizen *citizen = new TCheckCitizen;
	char lastName[SIZE_OF_CITIZEN_NAME];
	char lastFamily[SIZE_OF_CITIZEN_FAMILY];
	TGArray<TCheckCitizen> array;

	log(LOG_VERIFY, CM_ADMIN_CHECKOUT_CITIZENS, "Начало");
	h.printf("Проверка за подобни лица\n");
	h.printf("Начало: %2d.%02d.%02d\n", start.hour, start.minute, start.second);
	h.printf("\n");

	m.Begin(citizen->Name, "F_UCN, F_UCN_TYPE, F_NAME, F_RENAME, F_FAMILY");
	m.AddOrder("F_NAME, F_FAMILY, F_RENAME");

	*lastName = *lastFamily = '\0';
	TQuery q(str(m));
	while (q.Read())
	{
		*citizen << q;
		citizen->LoadAddrs();

		if (
			strcmp(citizen->family, lastFamily) ||
			strcmp(citizen->name, lastName)
		)
		{
			check_citizens(h, array);
			array.Flush();
			strcpy(lastFamily, citizen->family);
			strcpy(lastName, citizen->name);

			m.clear();
			m.printf("Проверка на '%s'...", citizen->name);
			wait.TickText(str(m));
		}

		array.Add(citizen);
		citizen = new TCheckCitizen;
	}
	delete citizen;
	wait.SetText(WAIT);
	check_citizens(h, array);

	CTime final = Now();
	h.printf("\nКрай: %2d.%02d.%02d\n", final.hour, final.minute, final.second);

	h.Show(&wait);
	log(LOG_VERIFY, CM_ADMIN_CHECKOUT_CITIZENS, "Край");
}

// ----- TCheckFirm ----------------------------------------------------------
class TCheckFirm : public TCheckAddress
{
public:
	TCheckFirm();
	static TGroup *Clone();

	char name[SIZE_OF_FIRM_NAME];
	char ufn[SIZE_OF_UCN];
	char bulstat[SIZE_OF_BULSTAT];

	char cName[SIZE_OF_FIRM_NAME];
	char affix[SIZE_OF_ALIAS];

	bool SimilarName(const TCheckFirm &that) const;		// 2011:119
	bool Similar(const TCheckFirm &that) const;
	void PrintF(THTMLFile &h) const;
	void SetCName();

	bool operator<=(const TCheckFirm &that) const { return strcmp(cName, that.cName) <= 0; }

protected:
	static void LoadDamns();
	static char damns[COMPANY_TYPES_MAX][SIZE_OF_ALIAS];
	static int damnc;
};

TCheckFirm::TCheckFirm()
	: TCheckAddress("T_FIRM", TCheckFirm::Clone)
{
	Add(
		new TString("F_NAME", name, sizeof name),
		new TString("F_UFN", ufn, sizeof ufn),
		new TString("F_BULSTAT", bulstat, sizeof bulstat),
		new TString("F_CNAME", cName, sizeof cName),
		NULL
	);
}

TGroup *TCheckFirm::Clone()
{
	return new TCheckFirm;
}

bool TCheckFirm::SimilarName(const TCheckFirm &that) const
{
	return any(affix) && strcmp(affix, that.affix) ? false : !strcmp(cName, that.cName);
}

bool TCheckFirm::Similar(const TCheckFirm &that) const
{
	return SimilarName(that) && TCheckAddress::Similar(that);
}

void TCheckFirm::PrintF(THTMLFile &h) const
{
	h.printf("%3s %10s %-50s", UCNType->Seek(ucnType), ucn, name);
	TCheckAddress::PrintF(h);
}

void TCheckFirm::SetCName()
{
	char *s;
	int i;

	LoadDamns();

	strcpy(cName, name);
	for (s = cName; *s; s++)
		if (!isalnum(*s))
			*s = ' ';

	*affix = '\0';
	for (i = 0; i < damnc; i++)
	{
		if ((s = strstr(cName, damns[i])) != NULL && (s == cName || isspace(s[-1])))
		{
			int len = strlen(damns[i]);

			if (s[len] == '\0' || isspace(s[len]))
			{
				strcpy(affix, damns[i]);
				memset(s, ' ', len);
				break;
			}
		}
	}

	stremove(cName, ' ');
}

char TCheckFirm::damns[COMPANY_TYPES_MAX][SIZE_OF_ALIAS];
int TCheckFirm::damnc = 0;

void TCheckFirm::LoadDamns()
{
	if (!damnc)
	{
		strcpy(damns[0], "ГР");
		strcpy(damns[1], "УЛ");
		strcpy(damns[2], "БУЛ");
		strcpy(damns[3], "ЗК");
		damnc = 4;

		// 2010:207 FloatCriteria() -> load
		TGArray<TCompanyType> damnTypes;

		CompanyType->LoadData(&damnTypes, NULL, FLAG_NULL);
		for (int i = 0; i < damnTypes.Count(); i++)
		{
			strcpy(damns[damnc++], damnTypes[i]->name);
			if (damnc == COMPANY_TYPES_MAX)
				break;
		}
	}

	if (damnc < COMPANY_TYPES_MAX)
		strcpy(damns[damnc++], "С");
}

static void check_firms(THTMLFile &h, TGSArray<TCheckFirm> &array)
{
	for (int i = 0; i < array.Count() - 1; i++)
	{
		bool printed = false;

		while (array[i]->Similar(*array[i + 1]))
		{
			if (!printed)
			{
				array[i]->PrintF(h);
				printed = true;
			}
			array[i + 1]->PrintF(h);
			array.Remove(i + 1);
			if (i == array.Count() - 1)
				break;
		}

		if (printed)
			h.putc('\n');
	}
}

void checkout_firms(TWindow *parent)
{
	CTime start = Now();
	THTMLFile h;
	h.OpenTemp();

	TWaitWindow wait(parent, WAIT);
	msql m;
	TGSArray<TCheckFirm> array;

	log(LOG_VERIFY, CM_ADMIN_CHECKOUT_FIRMS, "Начало");
	h.printf("Проверка за подобни лица\n");
	h.printf("Начало: %2d.%02d.%02d\n", start.hour, start.minute, start.second);
	h.printf("\n");

	m.Begin("T_FIRM", "F_UCN, F_UCN_TYPE, F_NAME");

	int count = 0;
	TQuery q(str(m));
	while (q.Read())
	{
		TCheckFirm *firm = new TCheckFirm;

		*firm << q;
		firm->SetCName();
		firm->LoadAddrs();
		array.Add(firm);

		if ((++count % 100) == 1)
		{
			char tName[SIZE_OF_FIRM_NAME];

			strcpy(tName, firm->name);
			tName[15] = '\0';
			if (strchr(tName, ' '))
				*strchr(tName, ' ') = '\0';

			m.clear();
			m.printf("Проверка на '%s'...", tName);
			wait.TickText(str(m));
		}
	}

	wait.SetText(WAIT);
	check_firms(h, array);

	CTime final = Now();
	h.printf("\nКрай: %2d.%02d.%02d\n", final.hour, final.minute, final.second);

	h.Show(&wait);
	log(LOG_VERIFY, CM_ADMIN_CHECKOUT_FIRMS, "Край");
}

bool banconsimilar(TGArray<TFirm> *firms)
{
	TCheckFirm firm;
	const char *bulstat = NULL;

	firm << (*firms)[0];
	firm.SetCName();

	for (int i = 1; i < firms->Count(); i++)
	{
		TCheckFirm tFirm;

		tFirm << (*firms)[i];
		tFirm.SetCName();

		// 2011:119 FIX: name only
		if (!firm.SimilarName(tFirm))
			return error("Избрани са юридически лица с различни имена.");

		if (any(tFirm.bulstat))
		{
			if (!bulstat)
				bulstat = tFirm.bulstat;
			else if (strcmp(tFirm.bulstat, bulstat))
				return error("Избрани са юридически лица с различни ЕИК/БУЛСТАТ.");
		}
	}

	return true;
}

bool similar_firname(TFirm *firm1, TFirm *firm2)
{
	TCheckFirm tFirm1, tFirm2;

	tFirm1 << firm1;
	tFirm1.SetCName();
	tFirm2 << firm2;
	tFirm2.SetCName();

	return tFirm1.SimilarName(tFirm2);
}

// ----- deliverers ----------------------------------------------------------
class TStrMapDupl : public TStreetMap
{
public:
	TStrMapDupl() { }

	const char *ucn1;
};

#define strmapd OFFICE_GROUP_CAST(TStrMapDupl)

static void strmapd_deliverer1(mstr &m, TGroup *group) { print_user(m, strmapd->ucn); }
static void strmapd_deliverer2(mstr &m, TGroup *group) { print_user(m, strmapd->ucn1); }

static void strmapd_duplicates(mstr &m, TGroup *group)
{
	if (strmapd->uclp != DEFAULT_UCLP)
		m.printf("%s ", UCLP->Seek(strmapd->uclp));
	strmapd->Print(m);
}

#undef strmapd

static const offield strmapd_fields[] =
{
	{ "F_DELIVERER_1", strmapd_deliverer1 },
	{ "F_DELIVERER_2", strmapd_deliverer2 },
	{ "F_DUPLICATES", strmapd_duplicates },
	{ NULL, NULL }
};

#define street OFFICE_GROUP_CAST(TStreet)

static void street_street(mstr &m, TGroup *group) { m.cat(street->name); }

#undef street

static const offield street_fields[] =
{
	{ "F_STREET", street_street },
	{ NULL, NULL }
};

class TDeliverer : public TUser
{
public:
	TDeliverer() { }

	TGSArray<TStreetMap> streetMaps;
};

void checkout_street_maps(TWindow *parent)
{
	TOffice office;

	if (office.Open("StrMapDupl.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");
		constant tr2start = office.Find("tr2 start");
		constant tr2final = office.Find("tr2 final");
		TStreet street;
		TGSArray<TStreet> streets;

		office.Emit(NULL, tr1start, null_fields, NULL);
		street.uclp = DEFAULT_UCLP;
		street.LoadFloat(&streets);

		TGArray<TDeliverer> deliverers;
		msql m(User->Name);
		TWaitWindow wait(parent, WAIT);

		m.AddChar("F_POST", POST_DELIVERER);
		m.AddLong("F_FLAGS", FLAG_ACTIVE);
		m.AddOrder("F_UCN");
		TQuery q(str(m));

		while (q.Read())
		{
			TDeliverer *deliverer = new TDeliverer;

			*deliverer << q;
			m.Begin(StreetMap->Name);
			m.AddString("F_UCN", deliverer->ucn);
			StreetMap->LoadArray(deliverer->streetMaps, str(m));

			for (int i = 0; i < deliverer->streetMaps.Count(); i++)
			{
				TStreetMap *streetMap = deliverer->streetMaps[i];

				for (int j = 0; j < deliverers.Count(); j++)
				{
					TDeliverer *tDeliverer = deliverers[j];
					int k = tDeliverer->streetMaps.Find(streetMap);

					if (k != INT_MAX)
					{
						TStreetMap *tStreetMap = tDeliverer->streetMaps[k];
						TStrMapDupl dupl;

						dupl << streetMap;
						dupl.ucn1 = tStreetMap->ucn;
						dupl.map.pad(LENGTH_OF_STREET_MAP);
						memset(ncstr(dupl.map), ' ', LENGTH_OF_STREET_MAP);

						for (k = 0; k < streetMap->map.len() && k < tStreetMap->map.len(); k++)
							if (streetMap->map[k] == NUMAP_ACTIVE && tStreetMap->map[k] == NUMAP_ACTIVE)
								ncstr(dupl.map)[k] = NUMAP_ACTIVE;

						dupl.Adjust();
						if (any(dupl.map))
							office.Emit(tr1start, tr1final, strmapd_fields, &dupl);
					}
				}

				street << streetMap;
				int k = streets.Find(&street);
				if (k != INT_MAX)
					streets.Remove(k);
			}

			deliverers.Add(deliverer);
		}

		office.Emit(tr1final, tr2start, null_fields, NULL);

		for (int i = 0; i < streets.Count(); i++)
			office.Emit(tr2start, tr2final, street_fields, streets[i]);

		office.Emit(tr2final, NULL, null_fields, NULL);
		office.Show(&wait);
	}
}
