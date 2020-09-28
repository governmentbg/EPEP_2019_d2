#include "cac.h"

// ----- TOutRegBookGroup ----------------------------------------------------
class TOutRegBookGroup : public TBookGroup
{
public:
	TOutRegBookGroup();

	virtual void FormCriteria(msql &m);
};

TOutRegBookGroup::TOutRegBookGroup()
	: TBookGroup("T_OUTREG", "F_COLLEGE", "F_DATE", false)
{
	Add(new TCheck("F_NEW_FORMAT", &flags, BOOK_NEW_FORMAT));
	flags = BOOK_NEW_FORMAT;
}

void TOutRegBookGroup::FormCriteria(msql &m)
{
	TBookGroup::FormCriteria(m);
	EndCriteria(m);
}

// ----- book_outreg ---------------------------------------------------------
#include "OutRegBookDialog.h"
#include "OutRegBookDialog.cpp"

#define outReg OFFICE_GROUP_CAST(TOutReg)

static void outreg_descript(mstr &m, TGroup *group)
{
	m.printf("%s", Kind->Seek(outReg->kind));

	if (any(outReg->text))
	{
		m.cat(" - ");
		m.cat(outReg->text);
	}

	TGArray<TInRegEvent> events;
	outReg->LoadInRegs(&events, "F_NO, F_YEAR", "F_KIND");

	for (int i = 0; i < events.Count(); i++)
	{
		TInRegEvent *inRegEvent = events[i];

		m.printf("\v%s № %ld/%ldг.", strchr(KIND_INREG_EVENTS, inRegEvent->kind) ? "Входящ документ" :
			Kind->Seek(inRegEvent->kind), inRegEvent->key.no, inRegEvent->key.year);
	}

}

static void outreg_returned(mstr &m, TGroup *group) { outReg->returned.Print(m, C_DATE_GOD); }
static void outreg_post(mstr &m, TGroup *group) { print_postal(m, outReg); }

// from querydoc:query_returneds(); hopefully querydoc will be html someday
static void query_resolution(mstr &m, char *name, char resolution)
{
	if (resolution != RESOLUTION_FAKE)
		m.printf("%s: %s\v", name, Resolution->Seek(resolution));
}

static void outreg_result(mstr &m, TGroup *group)
{
	char result = outReg->Status();

	if (strchr(RESULT_IOREG_RETURNEDS, result))
	{
		if (!strchr(RESULT_IOREG_FINISHEDS, result) && !strchr(RESULT_IOREG_CONTINUEDS, result))
			m.printf("Резултат: %s\v", Result->Seek(outReg->Status()));

		query_resolution(m, "Резултат от въззивна инстанция", outReg->resultI);
		query_resolution(m, "Резултат от касационна инстанция", outReg->resultKS);
		query_resolution(m, "II Резултат от въззивна инстанция", outReg->resultII);
		query_resolution(m, "Резултат за МП", outReg->resultMJ);

		if (any(outReg->vssIndex))
			m.printf("Резултат - индекс: %s\v", outReg->vssIndex);
	}
}

#undef outReg

static const offield outreg_fields[] =
{
	{ "F_NO",		keygroup_key_no },
	{ "F_DATE",		container_date },
	{ "F_DESCRIPT", 	outreg_descript },
	{ "F_LAWSUIT",	outreg_lawsuit },
	{ "F_RECEIVERS",	outreg_receivers },
	{ "F_RETURNED",	outreg_returned },
	{ "F_POST",		outreg_post },
	{ "F_RESULT",	outreg_result },
	{ NULL,		NULL }
};

class TOutRegBook : public TBook
{
public:
	TOutRegBook(TWindow *parent, TBookGroup *group, const char *fileName);

protected:
	virtual TGroup *Process(TQuery &q);
};

TOutRegBook::TOutRegBook(TWindow *parent, TBookGroup *group, const char *fileName)
	: TBook(parent, group, fileName, outreg_fields)
{
}

TGroup *TOutRegBook::Process(TQuery &q)
{
	TOutReg *outReg = new TOutReg;

	*outReg << q;
	MinMax(outReg->key.no, outReg->date);
	return outReg;
}

void book_outreg(TWindow *parent)
{
	TOutRegBookGroup group;

	if (TOutRegBookDialog(parent, &group).Execute() == IDOK)
		TOutRegBook(parent, &group, group.flags & BOOK_NEW_FORMAT ? "OutReg.htm" : "OutReg_.htm").Execute(BOOK_EXEC_HEADER);
}

void print_outreg(TWindow *parent, TOutReg *outReg, long flags)
{
	TWaitWindow wait(parent, WAIT);
	show_office(flags & BOOK_NEW_FORMAT ? "OutReg1.htm" : "OutReg1_.htm", outreg_fields, outReg);
}
