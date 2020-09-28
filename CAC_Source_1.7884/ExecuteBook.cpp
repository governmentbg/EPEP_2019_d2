#include "cac.h"

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
class TExecuteBookGroup : public TBookGroup
{
public:
	TExecuteBookGroup();

	virtual void FormCriteria(msql &m);

	long allColumns;

protected:
	virtual void Title(mstr &m);
};

TExecuteBookGroup::TExecuteBookGroup()
	: TBookGroup("T_JUDGEMENT J JOIN T_DECISION D ON D.F_NO = J.F_NO AND D.F_YEAR = J.F_YEAR AND D.F_TYPE = J.F_TYPE "
		"AND D.F_KIND = J.F_KIND AND D.F_DATE = J.F_DATE", "F_COLLEGE", "F_DATE", false)
{
	Add(new TCheck("F_ALL_COLUMNS", &allColumns, TRUE));
	allColumns = TRUE;
}

void TExecuteBookGroup::FormCriteria(msql &m)
{
	m.Begin(Name, "D.F_NO, D.F_YEAR, D.F_TYPE, D.F_KIND, J.F_UCN, J.F_UCN_TYPE, D.F_DATE, J.F_SEND_DATE, J.F_SEND_TO, "
		"J.F_SEND_TEXT, J.F_EXEC_TEXT, J.F_EXEC_PLACE, J.F_EXEC_BIND, J.F_EXEC_DATE, J.F_COUNT_DATE, J.F_FORGIVE, "
		"J.F_GIVEN_DATE, J.F_EXEC_REMARK, J.F_ORDER_FROM, J.F_FORCE_DATE AS F_JUD_FORCE_DATE, "
		"D.F_SESSION_KIND AS F_DEC_SES_KIND, D.F_EVENT_NO AS F_DEC_EVENT_NO");

	m.AddRange("J.F_SEND_DATE", minDate, maxDate);
	m.AddOrder("J.F_SEND_DATE, J.F_TYPE, J.F_YEAR, J.F_NO");	// == EndCriteria()
}

void TExecuteBookGroup::Title(mstr &m)
{
	m.cpy("КНИГА ЗА ИЗПЪЛНЕНИЕ НА ПРИСЪДИТЕ\v");
	book_dates(m, this);
}

class TBookJudgement : public TJudgement
{
public:
	TBookJudgement();

	char sesKind;
	long eventNo;
	CDate judForceDate;

	TProcess *process;
};

TBookJudgement::TBookJudgement()
{
	Add(
		new TChar("F_DEC_SES_KIND", &sesKind),
		new TLong("F_DEC_EVENT_NO", &eventNo),
		new TDate("F_JUD_FORCE_DATE", &judForceDate, false),
		NULL
	);
}

#define judgement OFFICE_GROUP_CAST(TBookJudgement)

static void execute_book_description(mstr &m, TGroup *group)
{
	print_name(m, judgement->ucn, judgement->ucnType);
	m.cat("\v");

	TAddress address;

	strcpy(address.ucn, judgement->ucn);
	address.ucnType = judgement->ucnType;

	if (address.Try())
		m.printf(" %s\v", UCLP->Seek(address.uclp));
	else
	{
		TForeignAddress foreignAddress;

		strcpy(foreignAddress.ucn, judgement->ucn);
		foreignAddress.ucnType = judgement->ucnType;

		if (foreignAddress.Try())
		{
			Country->Seek(foreignAddress.country);
			// 2012:166 TRQ: FIX: .address -> .foreign
			m.printf(" %s %s\v", Country->name, foreignAddress.foreign);
		}
		else
			judgement->process->dprintf("Липсва адрес на осъдения по %s %d/%d\n", Type->Seek(judgement->key.type), flag_key_no(&judgement->key),judgement->key.year);
	}

	// 2012:059 LRQ: FIX: always execText, never text
	m.cat(judgement->execText);
}

static void execute_book_force(mstr &m, TGroup *group)
{
	if (PRINT_JUDGEMENTS)
		m.printf("%s № %d", Kind->Seek(judgement->kind), judgement->eventNo);

	TLawsuit lawsuit;
	lawsuit.key = judgement->key;

	if (lawsuit.Try("F_KIND"))
	{
		m.printf(" %s %d/%d", lawsuit.Try() ? Kind->Seek(lawsuit.kind) : Type->Seek(judgement->key.type),
			flag_key_no(&lawsuit.key), lawsuit.key.year);
	}

	if (PRINT_COMPOSITIONS)
	{
		TSession session;

		session.key = judgement->key;
		session.date = judgement->date;
		session.kind = judgement->sesKind;

		if (session.Try("F_COMPOSITION"))
			m.printf(" %s с-в", Composition->Seek(session.composition));
		else
		{
			judgement->process->dprintf("Не е открито %s от %D по %s %d/%d\n",
				Kind->Seek(judgement->sesKind), &judgement->date, Type->Seek(judgement->key.type),
				flag_key_no(&judgement->key), judgement->key.year);
		}
	}

	judgement->judForceDate.Print(m, "\vз.с. %D");

	if (any(judgement->orderFrom))
		m.printf("\v%s", judgement->orderFrom);
}

static void execute_book_send(mstr &m, TGroup *group)
{
	if (any(judgement->sendTo))
		m.cpy(judgement->sendTo);

	m.printf(" %D ", &judgement->sendDate);

	if (any(judgement->sendTo))	// ??
	     m.cat(judgement->sendText);
}

static void execute_book_execute(mstr &m, TGroup *group)
{
	judgement->execDate.Print(m, "%D\v");
	m.cat(judgement->execBind);
	judgement->countDate.Print(m, "\v%D\v");
	m.cat(judgement->sendTo);
}

static void execute_book_pleace(mstr &m, TGroup *group) { m.cat(judgement->execPlace); }
static void execute_book_forgive(mstr &m, TGroup *group) { m.cat(judgement->forgive); }

static void execute_book_complete(mstr &m, TGroup *group)
{
	if (any(judgement->execBind))
		m.printf("С преписка No %s\v", judgement->execBind) ;

	if (any(judgement->sendTo))
		m.cat(judgement->sendTo);
}

static void execute_book_remark(mstr &m, TGroup *group)
{
	m.printf("Предадено %D", &judgement->givenDate);

	if (any(judgement->execRemark))
		m.printf(" %s", judgement->execRemark);
}

static void execute_form_title(mstr &m, TGroup *group)
{
	m.printf("%s %ld/%ldг., %s от %Dг., ", Type->Seek(judgement->key.type), flag_key_no(&judgement->key),
		judgement->key.year, Kind->Seek(judgement->kind), &judgement->date);
	judgement->PrintNames(m);
}

#undef judgement

static const offield execute_fields[] =
{
	{ "F_DESCRIPTION",execute_book_description },
	{ "F_FORCE",	execute_book_force },
	{ "F_SEND",		execute_book_send },
	{ "F_EXECUTE",	execute_book_execute },
	{ "F_PLACE",	execute_book_pleace },
	{ "F_FORGIVE",	execute_book_forgive },
	{ "F_COMPLETE",	execute_book_complete },
	{ "F_REMARK",	execute_book_remark },
	{ "F_FORM_TITLE",	execute_form_title },
	{ NULL, NULL }
};

#include "ExecuteBookDialog.h"
#include "ExecuteBookDialog.cpp"

class TExecuteBook : public TBook
{
public:
	TExecuteBook(TWindow *parent, TBookGroup *group, const char *fileName);

	virtual TGroup *Process(TQuery &q);
};

TExecuteBook::TExecuteBook(TWindow *parent, TBookGroup *group, const char *fileName)
	: TBook(parent, group, fileName, execute_fields, ::book_fields, BOOK_FIXED_DATES)
{
}

TGroup *TExecuteBook::Process(TQuery &q)
{
	TBookJudgement *judgement = new TBookJudgement;

	*judgement << q;
	judgement->process = this;
	MinMax(1, judgement->sendDate);
	return judgement;
}

void execute_book(TWindow *parent)
{
	TExecuteBookGroup group;

	if (TExecuteBookDialog(parent, &group).Execute() == IDOK)
		TExecuteBook(parent, &group, group.allColumns ? "ExecBook.htm" : "ExecBkSm.htm").Execute(BOOK_EXEC_HEADER);
}

void print_one_judgement(TWindow *, TJudgement *tJudgement)
{
	if (tJudgement->sendDate.Empty())
		error("Датата на изпращане не е попълнена.");
	else
	{
		TBookJudgement judgement;

		register_usage(REGIME_EXECUTION_BOOK_PATCH, tJudgement->key.type, 1, "Фиш присъда");
		judgement << tJudgement;
		show_office("ExecForm.htm", execute_fields, &judgement);
	}
}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
