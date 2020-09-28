#include "cac.h"

#if COURT_TYPE == COURT_AREAL

class TCitizenP24a1 : public TCitizen
{
public:
	TCitizenP24a1(TSideWind *sideWind) { *this << sideWind; }
	TRCDKey damnKey;
	char damnName[SIZE_OF_DAMN_NAME];
};

#define citizen OFFICE_GROUP_CAST(TCitizenP24a1)

static void p24a1_citizen(mstr &m, TGroup *group)
{
	m.printf("%s %s %s %s", citizen->name, citizen->reName, citizen->family, citizen->reFamily);
}

static void p24a1_damn_name(mstr &m, TGroup *group) { m.cpy(citizen->damnName); }
static void p24a1_company_type(mstr &m, TGroup *group) { m.cpy(CompanyType->Seek(citizen->damnKey.type)); }
static void p24a1_damn_key(mstr &m, TGroup *group) { m.printf("%d/%d", citizen->damnKey.no, citizen->damnKey.year); }

#undef citizen

static const offield p24a1_fields[] =
{
	{ "F_CITIZEN", p24a1_citizen },
	{ "F_NAME", p24a1_damn_name },
	{ "F_TYPE", p24a1_company_type },
	{ "F_KEY", p24a1_damn_key },
	{ NULL, NULL }
};

static void print_p24a1(TDamn *damn, TSideWind *sideWind)
{
	TCitizenP24a1 citizen(sideWind);

	if (citizen.Get())
	{
		citizen.damnKey = damn->key;
		citizen.damnKey.type = damn->companyType;
		strcpy(citizen.damnName, damn->name);
		show_office("P24a1.htm", p24a1_fields, &citizen);
	}
}

static bool print_p24a1(TWindow *parent)
{
	TDamn damn;

	if (get_ucn(parent, &damn, UCN_DAMNS))
	{
		TLawsuit lawsuit;
		lawsuit.key = damn.key;

		if (lawsuit.Get())
		{
			// 2003:155 LRQ: Slightly more accurate existence test
			// 2003:181 LRQ: more accurate: resurrect after cancel
			bool existing = true;
			TDecision decision;
			msql m(decision.Name);

			decision.key = lawsuit.key;
			m.Add(&decision, "F_NO, F_YEAR, F_TYPE");
			m.AddOrder("F_DATE");

			TQuery q(str(m));
			while (q.Read())
			{
				decision << q;

				// 2003:213 LRQ: count some sessions
				if (strchr(RESULT_SETTLEMENTS, decision.result))
				{
					TSession session;

					session.key = decision.key;
					session.date = decision.date;
					session.kind = decision.sessionKind;

					if (session.Get())
					{
						if (session.result == RESULT_CANCEL_REGISTER)
							existing = false;
						// 2006:074 LRQ
						else if (session.result == RESULT_FIRM_SENDSES)
							existing = false;
					}
				}

				if (strchr(RESULT_FIRM_UNACCEPTS, decision.result))
					existing = false;
				else if (strchr(RESULT_FIRM_REACCEPTS, decision.result))
					existing = true;
				else if (strchr(RESULT_FIRM_CANCELD1S, decision.result))
					existing = false;
				else if (strchr(RESULT_FIRM_RESURRECTS, decision.result))
					existing = true;
				// 2006:074 LRQ
				else if (strchr(RESULT_FIRM_SENDECS, decision.result))
					existing = false;
			}

			if (!existing)
			{
				THTMLFile h;

				h.OpenTemp();
				query_law(h, &lawsuit, true);
				h.Show();
			}
			else
			{
				TSideWind sideWind;

				// 2003:308 BRW: converted, but does it work?!
				if (select_citizenwind(parent, &sideWind, &lawsuit))
					print_p24a1(&damn, &sideWind);
			}
		}

		return true;
	}

	return false;
}

void query_p24a1(TWindow *parent)
{
	TFirm firm;

	if (REPEAT_P24A1S)
	{
		for (;;)
		{
			// 2004:163 LPR: unify UCN searches
			if (!get_ucn(parent, &firm, UCN_FIRMS) || p24a1tradealike(parent, &firm))
				if (!print_p24a1(parent))
					break;

			strcpy(firm.ucn, "");
		}
	}
	// 2004:163 LPR: unify UCN searches
	else if (!get_ucn(parent, &firm, UCN_FIRMS) || p24a1tradealike(parent, &firm))
		print_p24a1(parent);
}

#endif  // AREAL
