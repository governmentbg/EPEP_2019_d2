#include "cac.h"

class TImportantBookGroup : public TBookGroup
{
public:
	TImportantBookGroup();
};

TImportantBookGroup::TImportantBookGroup()
	: TBookGroup("T_LAWSUIT", "F_TYPE", "F_DATE", true)
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	strcpy(types, C2S[TYPE_PUNISHMENT_LAW]);
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
}

#include "ImportantBookDialog.h"
#include "ImportantBookDialog.cpp"

#define lawsuit OFFICE_GROUP_CAST(TLawsuit)

static void bookimportant_connect(mstr &m, TGroup *group)
{
	TSender sender;
	TConnect connect;

	connect.key = lawsuit->key;
	connect.connectType = CONNECT_TYPE_FLOOR_LAWSUIT;
	if (connect.Try())
	{
		m.printf("%s/%d-", connect.connectNo, connect.connectYear);
		strcpy(sender.ucn, connect.ucn);
		sender.ucnType = connect.ucnType;
		if (sender.Try())
			m.cat(sender.name);
	}
}

static void bookimportant_name(mstr &m, TGroup *group)
{
	TGArray<TSideWind> sideWinds;

	char *categories = load_sidebasic(lawsuit, sideWinds);

	for (int i = 0; i < sideWinds.Count(); i++)
		if (categories[i] == SIDEWIND_RIGHT)
			add_side_name(m, sideWinds[i]);

	delete[] categories;
}

static void bookimportant_lawsuit(mstr &m, TGroup *group)
{
	m.printf("%d/%d %s", lawsuit->key.no, lawsuit->key.year, Kind->Seek(lawsuit->kind));
}

static void bookimportant_judge(mstr &m, TGroup *group) { print_user(m, lawsuit->judge); }

static void bookimportant_session_dates(mstr &m, TGroup *group)
{
	TSession session;
	msql w;

	session << lawsuit;
	session.FloatCriteria(w, "F_DATE");
	TQuery q(str(w));

	while (q.Read())
	{
		session << q;
		m.sep(", ");
		session.date.Print(m);
	}
}

static void bookimportant_judgement(mstr &m, TGroup *group)
{
	TJudgement judgement;
	msql w(judgement.Name, "F_FORCE_DATE");
	int count = 0;

	w.Add(lawsuit, "F_NO, F_YEAR, F_TYPE");
	TQuery q(str(w));

	while (q.Read())
	{
		judgement << q;
		if (!judgement.forceDate.Empty())
			count++;
	}

	itom(count, m);
}

#undef lawsuit

static const offield bookimportant_fields[]=
{
	{ "F_CONNECT", bookimportant_connect },
	{ "F_NAME", bookimportant_name },
	{ "F_LAWSUIT", bookimportant_lawsuit },
	{ "F_JUDGE", bookimportant_judge },
	{ "F_SESSION_DATES", bookimportant_session_dates },
	{ "F_DATE", container_date },
	{ "F_JUDGEMENT", bookimportant_judgement },
	{ NULL, NULL }
};

class TImportantBook : public TBook
{
public:
	TImportantBook(TWindow *parent, TBookGroup *group);

protected:
	virtual TGroup *Process(TQuery &q);
};

TImportantBook::TImportantBook(TWindow *parent, TBookGroup *group)
	: TBook(parent, group, "ImpBook.htm", bookimportant_fields, ::book_fields, BOOK_FIXED_DATES)
{
}

TGroup  *TImportantBook::Process(TQuery &q)
{
	TLawsuit *lawsuit = new TLawsuit;
	*lawsuit << q;
	return lawsuit;
}

void important_book(TWindow *parent)
{
	TImportantBookGroup group;

	if (TImportantBookDialog(parent, &group).Execute() == IDOK)
		TImportantBook(parent, &group).Execute();
}
