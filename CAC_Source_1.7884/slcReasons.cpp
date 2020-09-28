#include "cac.h"

#define reason OFFICE_GROUP_CAST(TSLCReason)

static void reason_slc_type(mstr &m, TGroup *group) { m.cat(SLCType->Seek(reason->slcType)); }

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
static constant KIND_PSHMENT_LAWS = KIND_PUNISHMENT_LAWS;
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_ADMIN
static constant KIND_PSHMENT_LAWS = KIND_TRICKY_LAWS;
#endif  // ADMIN

static void reason_law_kinds(mstr &m, TGroup *group)
{
	if (strequal(reason->lawKinds, KIND_PSHMENT_LAWS))
		m.cat("Всички наказателни дела");
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	else if (strequal(reason->lawKinds, KIND_2ND_LAWSUITS))
		m.cat("Всички второинстанционни/касационни дела");
#endif  //
	else if (strequal(reason->lawKinds, KIND_LAWSUITS))
		m.cat("Всички видове дела");
	else
	{
		char lawKinds[SIZE_OF_LAW_KINDS * 2];

		strcpy(lawKinds, reason->lawKinds);
		strcat(lawKinds, KIND_PSHMENT_LAWS);

		if (strequal(lawKinds, KIND_LAWSUITS))
			m.cat("Всички не-наказателни дела");
		else
		{
			for (const char *s = reason->lawKinds; *s; s++)
			{
				m.sep(", ");
				m.cat(Kind->Seek(*s));
			}
		}
	}
}

static void slc_reason_book(mstr &m, TGroup *group) { m.cat(reason->flags & SLC_REASON_BOOK ? "р" : ""); }

#undef reason

static const offield reason_fields[] =
{
	{ "F_SLC_TYPE",	reason_slc_type },
	{ "F_ORDER",	NULL },
	{ "F_NAME",		NULL },
	{ "F_LAW_KINDS",	reason_law_kinds },
	{ "F_ACTIVE",	group_active },
	{ NULL, NULL }
};

static void print_slc_reasons(TSLCReason *reason, TGroupArray *array)
{
	TOffice office;

	if (office.Open("PrintSlc.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, reason_fields, reason);

		for (int i = 0; i < array->Count(); i++)
			office.Emit(tr1start, tr1final, reason_fields, (*array)[i]);

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(NULL);
	}
}

#include "EditSLCReasonDialog.h"
#include "EditSLCReasonDialog.cpp"

#include "SLCReasonListWindow.h"
#include "SLCReasonListWindow.cpp"

#include "SLCReasonListDialog.h"
#include "SLCReasonListDialog.cpp"

void edit_slc_reasons(TWindow *parent)
{
	TSLCReasonListDialog(parent).Execute();
}
