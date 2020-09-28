class TPrintStreetMap : public TStreetMap
{
public:
	TPrintStreetMap();

	bool operator<=(const TPrintStreetMap &that) const;

	char uclpName[SIZE_OF_UCLP_NAME];
};

TPrintStreetMap::TPrintStreetMap()
{
	Add(new TString("F_UCLP_NAME", uclpName, sizeof uclpName));
}

#define printmap OFFICE_GROUP_CAST(TPrintStreetMap)

static void printmap_deliverer_name(mstr &m, TGroup *group) { print_user(m, printmap->ucn); }
static void printmap_uclp(mstr &m, TGroup *group) { m.cpy(printmap->uclpName); }
static void printmap_street(mstr &m, TGroup *group) { m.cpy(printmap->streetName); }
static void printmap_adr_no(mstr &m, TGroup *group) { printmap->PrintMap(m); }

#undef printmap

static const offield printmap_fields[]=
{
	{ "F_DELIVERER_NAME", printmap_deliverer_name },
	{ "F_UCLP", printmap_uclp },
	{ "F_STREET", printmap_street },
	{ "F_ADR_NO", printmap_adr_no },
	{ NULL, NULL }
};

static const char *sortstr(const char *s)
{
	const char *ts = strchr(s, '.');

	if (ts)
		while (ts[1] && isspace(ts[1]))
			ts++;

	return ts && *ts ? ts : s;
}

bool TPrintStreetMap::operator<=(const TPrintStreetMap &that) const
{
	int compare = strcmp(sortstr(uclpName), sortstr(that.uclpName));
	return compare ? compare < 0 : strcmp(sortstr(streetName), sortstr(that.streetName)) <= 0;
}

void print_street_map(const char *ucn)
{
	msql m("T_STREET_MAP SM JOIN T_UCLP U ON U.F_UCLP = SM.F_UCLP "
		"JOIN T_STREET S ON S.F_UCLP = SM.F_UCLP AND S.F_STREET = SM.F_STREET",
		"SM.F_UCN, SM.F_UCLP, SM.F_STREET, SM.F_MAP, U.F_NAME AS F_UCLP_NAME, S.F_NAME AS F_STREET_NAME");
	TOffice office;
	TGSArray<TPrintStreetMap> array;

	m.AddString("F_UCN", ucn);
	TQuery q(str(m));

	while (q.Read())
	{
		TPrintStreetMap *streetMap = new TPrintStreetMap;
		*streetMap << q;
		array.Add(streetMap);
	}

	if (office.Open("StreetMap.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1break = office.Find("tr1 break");
		constant tr1final = office.Find("tr1 final");

		TPrintStreetMap streetMap;
		long uclp = 0;

		strcpy(streetMap.ucn, ucn);
		office.Emit(NULL, tr1start, printmap_fields, &streetMap);

		for (int i = 0; i < array.Count(); i++)
		{
			TPrintStreetMap *printmap = array[i];

			if (uclp != printmap->uclp)
			{
				office.Emit(tr1start, tr1break, printmap_fields, printmap);
				uclp = printmap->uclp;
			}

			office.Emit(tr1break, tr1final, printmap_fields, printmap);
		}

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(NULL);
	}
}
