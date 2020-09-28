#define webgroup OFFICE_GROUP_CAST(TXferWebGroup)

static void webgroup_criteria(mstr &m, TGroup *group)
{
	if (strequal(webgroup->lawKinds, KIND_LAWSUITS))
		m.cpy("Всички видове дела");
	else
		print_kinds_list(m, webgroup->lawKinds, false, false);

	m.cat("\v");
	print_kinds_list(m, webgroup->decKinds, false, false);
}

#undef webgroup

static const offield title_fields[] =
{
	{ "F_CRITERIA",	webgroup_criteria },
	{ NULL,		NULL }
};

#define xferWeb OFFICE_GROUP_CAST(TXferWebPrint)

static void xferweb_limited_long(mstr &m, TGroup *group)
{
	if (xferWeb->Limited())
	{
		m.sep(", ");
		m.cat(xferWeb->Limited() == -1 ? "Грешка при четене" : "Дело с ограничен достъп");
	}
}

static void xferweb_xfer_date(mstr &m, TGroup *group) { xferWeb->xferDate.Print(m, C_DATE_GOD); }

static void xferweb_decision_no(mstr &m, TGroup *group)
{
	if (xferWeb->decisionNo)
		m.printf("%ld/", xferWeb->decisionNo);
	else
		m.cpy(" от ");
}

static void xferweb_decision_date(mstr &m, TGroup *group) { xferWeb->decisionDate.Print(m, C_DATE_GOD); }
static void xferweb_motives_date(mstr &m, TGroup *group) { ((CDate &) xferWeb->motivesDate).Print(m, "Мотиви от %Dг."); }

static void xferweb_outreg_no(mstr &m, TGroup *group)
{
	if (xferWeb->outreg.no)
		m.printf("%ld/", xferWeb->outreg.no);
}

static void xferweb_outreg_date(mstr &m, TGroup *group) { xferWeb->outregDate.Print(m, C_DATE_GOD); }
static void xferweb_return_result(mstr &m, TGroup *group) { m.cpy(xferWeb->returnResult); }
static void xferweb_force_date(mstr &m, TGroup *group) { xferWeb->forceDate.Print(m, C_DATE_GOD); }

#undef xferWeb

static const offield xferweb_fields[] =
{
	{ "F_XFER_KIND",		xfer_xfer_kind },
	{ "F_NO",			xferweb_no },
	{ "F_YEAR",			xferweb_year },
	{ "F_LIMITED",		xferweb_limited_long },
	{ "F_DECISION_KIND",	xferweb_decision_kind },
	{ "F_DECISION_NO",	xferweb_decision_no },
	{ "F_DECISION_DATE",	xferweb_decision_date },
	{ "F_MOTIVES_DATE",	xferweb_motives_date },
	{ "F_OUTREG_NO",		xferweb_outreg_no },
	{ "F_OUTREG_DATE",	xferweb_outreg_date },
	{ "F_RETURN_RESULT",	xferweb_return_result },
	{ "F_FORCE_DATE",		xferweb_force_date },
	{ NULL,			NULL }
};

void print_webs(TXferWebGroup *group, TGroupArray *array)
{
	TOffice office;

	if (office.Open("XferActs.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, title_fields, group);

		for (int i = 0; i < array->Count(); i++)
			office.Emit(tr1start, tr1final, xferweb_fields, (*array)[i]);

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(NULL);
	}
}
