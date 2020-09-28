#include "cac.h"

#include "personal1.cpp"
#include "personal_t.c"

static void WfromC(wchar_t *ws, const char *s)
{
	size_t size = strlen(s) + 1;

	if (!MultiByteToWideChar(1251, 0, s, size, ws, size))
		error("√Â¯Í‡ %lu ÔË ÍÓÌ‚ÂÒËˇ Í˙Ï UNICODE.", GETLASTERROR);
}

static void CfromW(char *s, const wchar_t *ws)
{
	size_t size = lstrlenW(ws) + 1;

	if (!WideCharToMultiByte(1251, 0, ws, size, s, size, NULL, NULL))
		error("√Â¯Í‡ %lu ÔË ÍÓÌ‚ÂÒËˇ ÓÚ UNICODE.", GETLASTERROR);
}

#if PERSONAL_UNICODE
#define W(ws, s) WfromC((ws), (s))
#define C(s, ws) CfromW((s), (ws))
#else   // PERSONAL_UNICODE
#define W(ws, s) strcpy((ws), (s))
#define C(s, ws) strcpy((s), (ws))
#endif  // PERSONAL_UNICODE

static TTextPart *new_AP(const char *pref1, const char *pref2, int pref2opt, const char *part, int flag)
{
	static size_t ADDRESS_PART_SIZES[] = { 26, 26, 26, 31, 31, 31, 8, 2, 6, 3, 3, 4 };
	TTextPart *ap = new TTextPart;
	int index;

	W(ap->pref1, pref1);
	W(ap->pref2, pref2);
	ap->pref2opt = pref2opt;
	W(ap->part, part);
	for (index = 0; (1 << index) < flag; index++);
	ap->size = ADDRESS_PART_SIZES[index];
	ap->flag = flag;
	return ap;
}

static void load_origsides_add(msql &m, const char *table, const TRCDKey &key)
{
	m.Begin(table, "F_UCN, F_UCN_TYPE, F_INVOLVEMENT");
	m.AddLong("F_NO", key.no);
	m.AddLong("F_YEAR", key.year);
	m.AddChar("F_TYPE", key.type);
}

static void load_origsides_cat(mstr &m, const char *table, const TRCDKey &key)
{
	msql w;

	m.cat(" UNION ");
	load_origsides_add(w, table, key);
	m.cat(w);
}

static void load_origsides_new(TGArray<TSideWind> &sideWinds, const TRCDKeyContainer *container, TRCDKey &origin)
{
	msql m;

	if (!find_origin(container, origin))
		failure();

	load_origsides_add(m, "T_SIDE_WIND", origin);
	load_origsides_cat(m, "T_JUDGEMENT", origin);

	if (container->key != origin)
		load_origsides_cat(m, "T_SIDE_WIND", container->key);

	load_sidebasicv(sideWinds, str(m));
}

// ----- TPersonalData -------------------------------------------------------
class TPersonalCitizen : public TCitizen
{
public:
	TPersonalCitizen();

	char oldName[SIZE_OF_CITIZEN_NAME];
	char oldFamily[SIZE_OF_CITIZEN_FAMILY];
	char oldReName[SIZE_OF_CITIZEN_RENAME];
	char oldReFamily[SIZE_OF_CITIZEN_REFAMILY];
};

TPersonalCitizen::TPersonalCitizen()
{
	Add(
		new TName("F_OLD_NAME", oldName, sizeof oldName),
		new TName("F_OLD_FAMILY", oldFamily, sizeof oldFamily),
		new TName("F_OLD_RENAME", oldReName, sizeof oldReName),
		new TName("F_OLD_REFAMILY", oldReFamily, sizeof oldReFamily),
		NULL
	);
}

TPersonal::TPersonal(const TRCDKeyContainer *container, TStringArray *extraUcns)
{
	TRCDKey origin;
	TGArray<TSideWind> sideWinds;

	load_origsides_new(sideWinds, container, origin);
	//load_origsides(container, &sideWinds, origin);

	if (extraUcns)
	{
		for (int i = 0; i < extraUcns->Count(); i++)
		{
			if (atob((*extraUcns)[i]))
			{
				TSideWind *sideWind = new TSideWind;

				sideWind->ucnType = UCN_CITIZEN_UCN;
				strcpy(sideWind->ucn, (*extraUcns)[i]);
				sideWinds.Add(sideWind);
			}
		}
	}

	sides = new TTextSide[sideWinds.Count() + 1];
	sides[sideWinds.Count()].ucnType = 0;

	for (int i = 0; i < sideWinds.Count(); i++)
	{
		TSideWind *sideWind = sideWinds[i];
		TTextSide *side = sides + i;
		size_t nc = 0;

		W(side->ucn, sideWind->ucn);
		side->ucnType = sideWind->ucnType;
		*side->lnc = '\0';
		side->parts = NULL;

		#undef UCN_CITIZENS
		if (strchr(UCN_CITIZENS, sideWind->ucnType))
		{
		#if TESTVER
			msql m("T_CITIZEN C LEFT JOIN T_OLD_CITIZEN O ON O.F_UCN = C.F_UCN AND O.F_UCN_TYPE = C.F_UCN_TYPE",
				"C.*, O.F_NAME AS F_OLD_NAME, O.F_FAMILY AS F_OLD_FAMILY, O.F_RENAME AS F_OLD_RENAME, "
				"O.F_REFAMILY AS F_OLD_REFAMILY");

			m.AddString("C.F_UCN", sideWind->ucn);
			m.AddChar("C.F_UCN_TYPE", sideWind->ucnType);
			TQuery q(str(m));

			if (!q.Read())
				continue;

			TPersonalCitizen citizen;
			citizen << q;

			constant names[] = { citizen.reFamily, citizen.oldReFamily, citizen.family, citizen.oldFamily,
				citizen.reName, citizen.oldReName, citizen.name, citizen.oldName, NULL };

			for (size_t i = 0; names[i]; i++)
			{
				if (any(names[i]))
				{
					size_t n;

					W(side->names[nc], names[i]);

					for (n = 0; n < nc; n++)
						if (!strcmp_t(side->names[n], side->names[nc]))
							break;

					if (n == nc)
						nc++;
				}
			}
		#else  // TESTVER
			TCitizen citizen;

			citizen.ucnType = sideWind->ucnType;
			strcpy(citizen.ucn, sideWind->ucn);

			citizen.Get();
			W(side->lnc, citizen.lnc);

			if (any(citizen.reFamily))
				W(side->names[nc++], citizen.reFamily);
			W(side->names[nc++], citizen.family);
			if (any(citizen.reName))
				W(side->names[nc++], citizen.reName);
			W(side->names[nc++], citizen.name);
		#endif  // TESTVER

			// 2014:350
			if (!sideWind->involvement)
			{
				side->parts = new TTextPart[1];
				side->parts->size = 0;
			}
		}
		else
		{
			#undef UCN_FIRMS
			// TODO? support TOldFirm
			if (strchr(UCN_FIRMS, sideWind->ucnType))
			{
				TFirm firm;

				firm.ucnType = sideWind->ucnType;
				strcpy(firm.ucn, sideWind->ucn);

				if (firm.Try() && *compress(firm.name, "\"'"))
				{
					char *s, *name;
					// not TODO: last name to first: we check all names anyway
					for (name = firm.name; (s = strchr(name, ' ')) != NULL; name = s + 1)
					{
						*s = '\0';
						W(side->names[nc++], name);
					}

					W(side->names[nc++], name);
				}
			}
		}

		*side->names[nc] = '\0';
	}

	repairs.key = origin;
	repairs.Try();

	pairs = new TTextPair[repairs.pairs.Count() + 1];
	*pairs[repairs.pairs.Count()].find = '\0';

	for (int i = 0; i < repairs.pairs.Count(); i++)
	{
		W(pairs[i].find, repairs.pairs[i]->find);
		W(pairs[i].replace, repairs.pairs[i]->replace);
	}

	initialize_sides_t(sides);
}

TPersonal::~TPersonal()
{
	delete[] pairs;

	for (TTextSide *side = sides; side->ucnType; side++)
		delete[] side->parts;

	delete[] sides;
}

struct TPersonalContext
{
	TPersonal *personal;
	IDispatch *iActive;
};

static int FilterReplace(void *data, const char_t *text, int start, int len, const char_t *rep, int replen)
{
	IDispatch *iActive = ((TPersonalContext *) data)->iActive;
	return filter_replace(iActive, text, start, len, rep, replen);
}

void TPersonal::Filter(char *text, int strenght, IDispatch *iActive)
{
	TPersonalContext context2 = { this, iActive };
	TTextContext context = { GetParts, iActive ? FilterReplace : NULL, &context2 };

#if PERSONAL_UNICODE
	TWSTR wtext(text);

	filter_ex_t(sides, wtext.wstr(), strenght, CourtType == COURT_MILITARY || CourtType == COURT_MILITARY_APPEAL,
		FILTER_JUDICIAL, &context, pairs);
	CfromW(text, wtext.wstr());
#else  // PERSONAL_UNICODE
	filter_ex_t(sides, text, strenght, CourtType == COURT_MILITARY || CourtType == COURT_MILITARY_APPEAL, FILTER_JUDICIAL,
		&context, pairs);
#endif  // PERSONAL_UNICODE
}

void TPersonal::Filter(wchar_t *text, int strenght, IDispatch *iActive)
{
	TPersonalContext context2 = { this, iActive };
	TTextContext context = { GetParts, iActive ? FilterReplace : NULL, &context2 };

#if PERSONAL_UNICODE
	filter_ex_t(sides, text, strenght, CourtType == COURT_MILITARY || CourtType == COURT_MILITARY_APPEAL, FILTER_JUDICIAL,
		&context, pairs);
#else  // PERSONAL_UNICODE
	char *ctext = new char[lstrlenW(text) + 1];

	CfromW(ctext, text);
	filter_ex_t(sides, ctext, strenght, CourtType == COURT_MILITARY || CourtType == COURT_MILITARY_APPEAL, FILTER_JUDICIAL,
		&context, pairs);
	WfromC(text, ctext);
#endif  //
}

TTextPart *TPersonal::GetParts(void *data, int index)
{
	TPersonal *personal = ((TPersonalContext *) data)->personal;
	const TTextSide *side = personal->sides + index;
	TAddress address;
	msql m;

	address.ucnType = (char) side->ucnType;
	C(address.ucn, side->ucn);
	address.FloatCriteria(m);

	TIArray<TTextPart> parts;
	TQuery q(str(m));

	while (q.Read())
	{
		// 2014:220 TRQ/URQ: don't Seek(), but display error messages
		const char *uclpName = "";
		const char *areaName = "";
		const char *regionName = "";

		address << q;
		UCLP->uclp = address.uclp;

		if (UCLP->Try())
		{
			uclpName = UCLP->name;

			Area->area = UCLP->area;
			if (Area->Get())
				areaName = Area->name;

			Region->area = UCLP->area;
			Region->region = UCLP->region;
			if (Region->Get())
				regionName = Region->name;

		}
		else
			error("%s: %ld: %s.", UCLP->Name, address.uclp, MISSING_DATA);

		parts.Add(new_AP("Œ¡À¿—“", "Œ¡À", PREF2_DOT, areaName, ADDRESS_PART_AREA));
		parts.Add(new_AP("Œ¡Ÿ»Õ¿", "Œ¡Ÿ.", PREF2_NULL, regionName, ADDRESS_PART_REGION));
		parts.Add(new_AP("√–¿ƒ", "√–", PREF2_DOT | PREF2_STRICT, uclpName, ADDRESS_PART_UCLP));
		parts.Add(new_AP("—≈ÀŒ", "—.", PREF2_STRICT, uclpName, ADDRESS_PART_UCLP));
		parts.Add(new_AP("Ã≈—“ÕŒ—“", "Ã.", PREF2_STRICT, uclpName, ADDRESS_PART_UCLP));
		parts.Add(new_AP("–¿…ŒÕ", "–.", PREF2_STRICT, "", ADDRESS_PART_DISTRICT));

		const char *streetName = "";

		if (address.street)
		{
			Street->uclp = address.uclp;
			Street->street = address.street;

			if (Street->Try())
				streetName = Street->name;
			else
				error("%s: %ld %ld: %s.", Street->Name, address.uclp, address.street, MISSING_DATA);
		}

		parts.Add(new_AP(" ¬¿–“¿À", " ¬", PREF2_DOT | PREF2_STRICT, streetName, ADDRESS_PART_QUARTER));
		parts.Add(new_AP("∆. .", "∆ ", PREF2_DOT, streetName, ADDRESS_PART_QUARTER));
		parts.Add(new_AP("”À»÷¿", "”À", PREF2_DOT, streetName, ADDRESS_PART_STREET));
		parts.Add(new_AP("¡”À≈¬¿–ƒ", "¡”À", PREF2_DOT, streetName, ADDRESS_PART_STREET));
		parts.Add(new_AP("ÕŒÃ≈–", "No", PREF2_DOT | PREF2_NO, address.adrNo, ADDRESS_PART_ADR_NO));
		parts.Add(new_AP("π", "#", PREF2_NO, address.adrNo, ADDRESS_PART_ADR_NO));
		parts.Add(new_AP("œŒƒÕŒÃ≈–", "œŒƒÕ", PREF2_DOT | PREF2_NO, address.subNo, ADDRESS_PART_SUB_NO));
		parts.Add(new_AP("¡ÀŒ ", "¡À", PREF2_DOT, address.blockNo, ADDRESS_PART_BLOCK_NO));
		parts.Add(new_AP("¬’Œƒ", "¬’", PREF2_DOT, address.entrance, ADDRESS_PART_ENTRANCE));
		parts.Add(new_AP("≈“¿∆", "≈“", PREF2_DOT, address.floor, ADDRESS_PART_FLOOR));
		parts.Add(new_AP("¿œ¿–“¿Ã≈Õ“", "¿œ", PREF2_DOT, address.apartment, ADDRESS_PART_APARTMENT));
	}

	TTextPart *tParts = new TTextPart[parts.Count() + 1];
	tParts[parts.Count()].size = 0;

	for (int i = 0; i < parts.Count(); i++)
		memcpy(tParts + i, parts[i], sizeof(TTextPart));

	return tParts;
}

void filter_personal(const TRCDKeyContainer *container, char *text, int strenght, IDispatch *iActive)
{
	TPersonal(container).Filter(text, strenght, iActive);
}

void filter_personal(const TRCDKeyContainer *container, mstr &text, int strenght)
{
	TPersonal(container).Filter(text, strenght);
}
