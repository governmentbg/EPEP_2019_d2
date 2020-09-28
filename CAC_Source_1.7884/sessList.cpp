#include "cac.h"

//---------- CONTENTS_ ------
enum
{
	CONTENTS_1	= 1,
	CONTENTS_2	= 2,
	CONTENTS_3	= 3,
	CONTENTS_4	= 4,
	CONTENTS_41	= 5,
	CONTENTS_5	= 6,
	CONTENTS_6	= 7
};

//---------- PAGE_ ----------
enum
{
	PAGE_SIZE		= 40,
	PAGE_HALF_SIZE	= PAGE_SIZE / 2
};

class TPrintSessionGroup : public TFloatGroup
{
public:
	TPrintSessionGroup();

	virtual void FormCriteria(msql &m);

	char lawKinds[SIZE_OF_KINDS];
	CDate minDate;
	CDate maxDate;
	char compositions[SIZE_OF_COMPOSITIONS];
	long hall;
	long partOfDay;
	CTime time;
	long withTypes;
	long variant;
	long lines;
	long check;
//	int selCount;
	CDate sesDate;
};

TPrintSessionGroup::TPrintSessionGroup()
	: TFloatGroup("T_SESSION S JOIN T_LAWSUIT L ON L.F_NO = S.F_NO AND L.F_YEAR = S.F_YEAR AND L.F_TYPE = S.F_TYPE")//,
		// 2018-03-27: LPR: comout, unneeded?
		//selCount(0)
{
	Add(
		new TCharList("F_KINDS", lawKinds, sizeof lawKinds),
		new TDate("F_MIN_DATE", &minDate, true),
		new TDate("F_MAX_DATE", &maxDate, true),
		new TCharList("F_COMPOSITIONS", compositions, sizeof compositions),
		new TLongAlias("F_HALL", &hall),
		new TRadio("F_WHOLE_DAY", &partOfDay, DAY_WHOLE),
		new TRadio("F_MORNING", &partOfDay, DAY_MORNING),
		new TRadio("F_AFTERNOON", &partOfDay, DAY_AFTERNOON),
		new TTime("F_TIME", &time),
		new TRadio("F_WITH_TYPE", &withTypes, TRUE),
		new TRadio("F_WITH_COLL", &withTypes, FALSE),
		new TRadio("F_VAR_1", &variant, CONTENTS_1),
		new TRadio("F_VAR_2", &variant, CONTENTS_2),
		new TRadio("F_VAR_3", &variant, CONTENTS_3),
		new TRadio("F_VAR_4", &variant, CONTENTS_4),
		new TRadio("F_VAR_41", &variant, CONTENTS_41),
		new TRadio("F_VAR_5", &variant, CONTENTS_5),
		new TRadio("F_VAR_6", &variant, CONTENTS_6),
		new TLong("F_LINES", &lines),
		new TCheck("F_ARL", &check, TRUE),
		NULL
	);

	partOfDay = DAY_WHOLE;
	withTypes = FALSE;
#if COURT_TYPE == COURT_ADMIN
	variant = CONTENTS_4;
#else
	variant = CONTENTS_1;
#endif
}

void TPrintSessionGroup::FormCriteria(msql &m)
{
	m.Begin(Name, "S.F_NO, S.F_YEAR, S.F_TYPE, L.F_KIND, S.F_TIME, L.F_JUDGE, L.F_SUBJECT, S.F_END, "
		// 2017:177 plain text -> JUROR_NAMES
		//"S.F_COMPOSITION AS F_SES_COMPOSITION, S.F_DATE AS F_SES_DATE, S.F_JUDGE AS F_SES_JUDGE, S.F_JUROR, "
		//"S.F_REJUROR, S.F_JUROR_X, S.F_RESERVE, S.F_FLAGS");
		"S.F_COMPOSITION AS F_SES_COMPOSITION, S.F_DATE AS F_SES_DATE, S.F_JUDGE AS F_SES_JUDGE, "
		JUROR_NAMES_QUOTED("S.") ", S.F_FLAGS");

	m.AddChars("L.F_KIND", lawKinds, KIND_LAWSUITS);
	m.AddRange("S.F_DATE", minDate, maxDate);
	m.AddCompos("S.F_COMPOSITION", compositions);
	m.AddChars("S.F_KIND", KIND_OPEN_SESSIONS, KIND_LAWSUITS);
	m.AddChars("NOT S.F_RESULT", RESULT_UNLINKEDS);

	if (hall)
		m.AddLong("S.F_HALL", hall);

	m.AddAmPm("S.F_TIME", time, partOfDay);
	m.AddOrder(variant == CONTENTS_5 || variant == CONTENTS_6 ?
		"S.F_DATE, S.F_COMPOSITION, S.F_TIME, S.F_YEAR, S.F_NO" : "S.F_DATE, S.F_TIME, S.F_YEAR, S.F_NO");
}

// ----- TListSession --------------------------------------------------------
class TListSession : public TSession
{
public:
	TListSession();

	virtual void Print(mstr &m);

	TConnect connect;
	long subject;
	int no;
	bool emptyRow;
	long subcount;
};

TListSession::TListSession()
{
	Rename("F_JUDGE", "F_SES_JUDGE");
	Rename("F_COMPOSITION", "F_SES_COMPOSITION");
	Rename("F_DATE", "F_SES_DATE");
	Add(new TLong("F_SUBJECT", &subject));
	Add(new TLong("F_SUBCOUNT", &subcount));

	no = 0;
	emptyRow = false;
	subcount = 1;
}

void TListSession::Print(mstr &m)
{
	m.printf("%-20s  %5ld/%4ld  %-T  %-D", Type->Seek(key.type), key.no, key.year, &time, &date);
}

// ----- TReorderSessionListBox ----------------------------------------------
class TReorderSessionListBox : public TGroupListBox
{
public:
	TReorderSessionListBox(TWindow* parent, int resourceId, TGArray<TListSession> &array);

	void Move(int delta);		// must be Movable()
	bool Movable(int delta);

protected:
	TListSession *SessionAt(int i) { return (TListSession *) GroupAt(i); }
	virtual TGroup *NewGroup() const;
};

TReorderSessionListBox::TReorderSessionListBox(TWindow* parent, int resourceId, TGArray<TListSession> &array)
	: TGroupListBox(parent, resourceId, array)
{
}

void TReorderSessionListBox::Move(int delta)
{
	int source = GetVisualIndex();
	int target = source + delta;
	TListSession session;

	session << SessionAt(source);
	*SessionAt(source) << SessionAt(target);
	*SessionAt(target) << session;

	RedrawVisual(source);
	RedrawVisual(target);
	SetVisualIndex(target);

	if (GetTopIndex() > 0)
		SetTopIndex(target - 12);
}

bool TReorderSessionListBox::Movable(int delta)
{
	int source = GetVisualIndex();
	int target = source + delta;

	return source >= 0 && target >= 0 && target < array->Count() &&
		SessionAt(source)->date == SessionAt(target)->date &&
		SessionAt(source)->time == SessionAt(target)->time;
}

TGroup *TReorderSessionListBox::NewGroup() const
{
	return new TListSession;
}

// ----- TReorderSessionDialog -----------------------------------------------
class TReorderSessionDialog : public TQuickDialog
{
public:
	TReorderSessionDialog(TWindow* parent, TGArray<TListSession> &array);

protected:
	TReorderSessionListBox *sessions;

	void OKBNClicked();
	void MoveUpBNClicked();
	void MoveUpBNEnable(TCommandEnabler &tce);
	void MoveDownBNClicked();
	void MoveDownBNEnable(TCommandEnabler &tce);

DECLARE_RESPONSE_TABLE(TReorderSessionDialog);
};

DEFINE_RESPONSE_TABLE1(TReorderSessionDialog, TQuickDialog)
	EV_BN_CLICKED(IDC_SESS_LIST_MOVE_UP, MoveUpBNClicked),
	EV_COMMAND_ENABLE(IDC_SESS_LIST_MOVE_UP, MoveUpBNEnable),
	EV_BN_CLICKED(IDC_SESS_LIST_MOVE_DOWN, MoveDownBNClicked),
	EV_COMMAND_ENABLE(IDC_SESS_LIST_MOVE_DOWN, MoveDownBNEnable),
END_RESPONSE_TABLE;

//{{TListSessionListDialog Implementation}}

TReorderSessionDialog::TReorderSessionDialog(TWindow* parent, TGArray<TListSession> &array)
:
	TQuickDialog(parent, IDD_REORDER_SESSION)
{
	sessions = new TReorderSessionListBox(this, IDC_SESS_LIST_REORDER, array);
}

void TReorderSessionDialog::MoveUpBNClicked()
{
	sessions->Move(-1);
}

void TReorderSessionDialog::MoveUpBNEnable(TCommandEnabler &tce)
{
	tce.Enable(sessions->Movable(-1));
}

void TReorderSessionDialog::MoveDownBNClicked()
{
	sessions->Move(+1);
}

void TReorderSessionDialog::MoveDownBNEnable(TCommandEnabler &tce)
{
	tce.Enable(sessions->Movable(+1));
}

#define printses OFFICE_GROUP_CAST(TPrintSessionGroup)

static void pls_title1(mstr &m, TGroup *group)
{
	m.cpy(printses->withTypes ? "на" : "на делата на");
}

static void pls_title2(mstr &m, TGroup *group)
{
	if (PRINT_COLLEGE)
		print_kinds_list(m, printses->lawKinds, true, !printses->withTypes);
}

static void pls_date(mstr &m, TGroup *group) { printses->sesDate.Print(m); }

static void pls_title5(mstr &m, TGroup *group)
{
	print_compositions_list(m, printses->compositions);

	// 2011:314 LPR: comout NO_MAX check, WTF was that for?
	if (printses->hall)	// && printses->hall != NO_MAX)
		m.printf(" %s.", Hall->Seek(printses->hall));
}

#undef printses

#define session OFFICE_GROUP_CAST(TListSession)

//---------------------------------- htm ------------------------------------
static void pls_no(mstr &m, TGroup *group) { itom(session->no, m); }
static void pls_session_time(mstr &m, TGroup *group) { session->time.Print(m); }
static void pls_lawsuit_kind(mstr &m, TGroup *group) { m.cat(KindAlias->Seek(session->kind)); }
static void pls_judge(mstr &m, TGroup *group) { print_user(m, session->judge); }
static void pls_subject(mstr &m, TGroup *group) { m.cpy(Subject->Seek(session->key.type, session->subject)); }

static void pls_lower_kind(mstr &m, TGroup *group)
{
	if (session->connect.connectKind)
	{
		ConnectKind->Seek(session->connect.connectKind);
		m.cat(ConnectKind->alias);
	}
}

static void pls_lower_no(mstr &m, TGroup *group)
{
	if (any(session->connect.connectNo))
		m.printf("%s/", session->connect.connectNo);
}

static void pls_lower_year(mstr &m, TGroup *group) { itom(session->connect.connectYear, m); }

static void pls_lower_court(mstr &m, TGroup *group)
{
	TSender sender;

	strcpy(sender.ucn, session->connect.ucn);
	sender.ucnType = session->connect.ucnType;

	if (session->connect.ucn && session->connect.ucnType && sender.Try())
		m.cpy(sender.name);
}

static void pls_lower_composition(mstr &m, TGroup *group)
{
	if (session->connect.composition)
		m.cpy(session->connect.CompositionName());
}

static void pls_composition(mstr &m, TGroup *group)
{
	if (session->composition)
		m.cpy(Composition->Seek(session->composition));
}

static void pls_parquet(mstr &m, TGroup *group)
{
	TSender sender;

	strcpy(sender.ucn, session->connect.ucn);
	sender.ucnType = session->connect.ucnType;

	if (session->connect.ucn && session->connect.ucnType)
		sender.Try();
	if (session->connect.key.no)
		m.printf("ПД № %s/%d %s", session->connect.connectNo, session->connect.connectYear, sender.name);
}

static void pls_jurors(mstr &m, TGroup *group)
{
	msql m_resul, m_flag;
	m_flag.Begin("T_SESSION", JUROR_NAMES_UNQUOTED ", F_FLAGS, F_RESULT");
	m_flag.AddLong("F_NO", session->key.no);
	m_flag.AddLong("F_YEAR", session->key.year);
	m_flag.AddChar("F_TYPE", session->key.type);
	m_flag.AddFlags("F_FLAGS", SESSION_PROCEED, SESSION_PROCEED, SESSION_ALL);
	m_flag.printf(" AND F_DATE <= %ld", session->date.Value());
	m_flag.AddOrder("F_DATE DESC");

	m_resul.Begin("T_SESSION", JUROR_NAMES_UNQUOTED ", F_FLAGS, F_RESULT");
	m_resul.AddLong("F_NO", session->key.no);
	m_resul.AddLong("F_YEAR", session->key.year);
	m_resul.AddChar("F_TYPE", session->key.type);
	m_resul.printf(" AND F_DATE <= %ld", session->date.Value());
	m_resul.AddOrder("F_DATE DESC");

	TQuery q_flag(str(m_flag));
	TSession resul_session, flag_session;

	while (q_flag.Read())
	{
		TQuery q_resul(str(m_resul));
		while (q_resul.Read())
		{
			flag_session << q_flag;
			resul_session << q_resul;
			if (!strchr(RESULT_UNCARRIEDS, resul_session.result))
			{
				if(!strcmp(flag_session.juror, resul_session.juror)
				&& !strcmp(flag_session.reJuror, resul_session.reJuror)
				&& !strcmp(flag_session.jurorX, resul_session.jurorX)
				&& !strcmp(flag_session.reserve, resul_session.reserve))
				{
				#if 20170174
					constant jurors[JUROR_COUNT] = { resul_session.juror, resul_session.reJuror,
						resul_session.jurorX, resul_session.reserve };

					for (int i = 0; i < JUROR_COUNT; i++)
					{
						if (atob(jurors[i]))
						{
							m.sep("\v");
							print_ucn_short(m, jurors[i]);
						}
					}
				#else  // 20170174
					print_ucn_short(m, resul_session.juror);
					if (any(resul_session.juror) && (any(resul_session.reJuror) || any(resul_session.jurorX))) m.cat("\v");
					print_ucn_short(m, resul_session.reJuror);
					if (any(resul_session.reJuror) && any(resul_session.jurorX)) m.cat("\v");
					print_ucn_short(m, resul_session.jurorX);
				#endif  // 20170174
				}
				break;
			}
		}
		break;
	}
}

static void pls_lawsuit(mstr &m, TGroup *group)
{
	m.printf("%ld/%ld", flag_key_no(&session->key), session->key.year);

	if (session->end > session->date)
		m.cat("&");
}

#undef session

static const offield other_fields[]=
{
	{ "F_NO", pls_no },
	{ "F_SUBCOUNT", NULL },
	{ NULL, NULL }
};

static const offield title_fields[]=
{
	{ "F_HEADER_1", pls_title1 },
	{ "F_HEADER_2", pls_title2 },
	{ "F_DATE", pls_date },
	{ "F_HEADER_5", pls_title5 },
	{ "F_JUDGE", generic_judge },
	{ NULL, NULL }
};

static const offield pls_fields[]=
{
	{ "F_NO", pls_no },
	{ "F_SUBCOUNT", NULL },
	{ "F_LAWSUIT_KIND", container_kind },
	{ "F_SESSION_TIME", pls_session_time },
	{ "F_JUDGE", pls_judge },
	{ "F_LAWSUIT_KIND", pls_lawsuit_kind },
	{ "F_SUBJECT", pls_subject },
	{ "F_LOWER_KIND", pls_lower_kind },
	{ "F_LOWER_NO", pls_lower_no },
	{ "F_LOWER_YEAR", pls_lower_year },
	{ "F_LOWER_COURT", pls_lower_court },
	{ "F_LOWER_COMPOSITION", pls_lower_composition },
	{ "F_COMPOSITION", pls_composition },
	{ "F_FLOOR_LAWSUIT", pls_parquet },
	{ "F_JURORS", pls_jurors },
	{ "F_LAWSUIT", pls_lawsuit },
	{ NULL, NULL }
};

static void session_print_list(TWindow* parent, const char *criteria, TPrintSessionGroup *group)
{
	static constant names[] = { "", "SLstV1.htm", "SLstV2.htm", "SLstV3.htm", "SLstV4.htm", "SLstV41.htm", "SLstV5.htm",
		"SLstV6.htm" };

	TWaitWindow wait(parent, WAIT);
	TWindowHideParent hide(parent);

	TOffice office;
	TConnect connect;
	TGArray<TListSession> sessionList;
	TQuery q(criteria);

	while (q.Read())
	{
		TListSession *session = new TListSession;

		*session << q;
		sessionList.Add(session);
		wait.TickCount(sessionList.Count(), "заредени заседания");
	}

	if ((!group->check || TReorderSessionDialog(parent, sessionList).Execute() == IDOK) &&
		office.Open(names[group->variant]))
	{
		bool two_column = group->variant == CONTENTS_1 || group->variant == CONTENTS_2;
		bool forty1 = group->variant == CONTENTS_4 || group->variant == CONTENTS_41;
		constant hd1start = office.Find("hd1 start");
		constant hd1break = two_column ? office.Find("hd1 break") : NULL;
		constant tr1start = office.Find("tr1 start");
		constant tr1break = two_column ? office.Find("tr1 break") : NULL;
	#if COURT_TYPE == COURT_APPEAL
		constant tr1no = forty1 ? office.Find("tr1 no") : NULL;
		constant tr1lower = forty1 ? office.Find("tr1 lower") : NULL;
	#endif  // APPEAL
		constant tr1final = office.Find("tr1 final");
		constant hd1final = office.Find("hd1 final");

		office.Emit(NULL, hd1start, null_fields, NULL);
		bool firstList = true;
		long fullCount = sessionList.Count();

		while (sessionList.Count())
		{
			TGArray<TListSession> sessList;

			sessList.Add(sessionList.Detach(0));
			CTime time = sessList[0]->time;

			if (firstList)
				firstList = false;
			else
				office.EmitBreak();

			while (sessionList.Count() && sessionList[0]->date == sessList[0]->date)
			{
				TListSession *sess = sessionList.Detach(0);

				if (sess->time == time)
					sess->time.Clear();
				else
				{
					for (int i = 0; i<group->lines; i++)
					{
						TListSession *line = new TListSession;
						line->emptyRow = true;
						sessList.Add(line);
					}
					time = sess->time;
				}

				sessList.Add(sess);
			}

			group->sesDate = sessList[0]->date;
			office.Emit(hd1start, tr1start, title_fields, group);

			if (two_column)
			{
				while (sessList.Count() % PAGE_SIZE)
				{
					TListSession *line = new TListSession;
					line->emptyRow = true;
					sessList.Add(line);
				}

				for (int page = 0; page < sessList.Count(); page += PAGE_SIZE)
				{
					if (page)
						office.EmitBreak(hd1start, hd1break);

					for (int element = page; element < page + PAGE_HALF_SIZE; element++)
					{
						sessList[element]->no = element + 1;
						office.Emit(tr1start, tr1break, sessList[element]->emptyRow ?
							other_fields : pls_fields, sessList[element]);

						sessList[element + PAGE_HALF_SIZE]->no = element + PAGE_HALF_SIZE + 1;
						office.Emit(tr1break, tr1final, sessList[element + PAGE_HALF_SIZE]->emptyRow ?
							other_fields : pls_fields, sessList[element + PAGE_HALF_SIZE]);
					}
				}
			}
			else
			{
				for (int x = 0; x < sessList.Count(); x++)
				{
					TListSession *session = sessList[x];
				#if COURT_TYPE == COURT_APPEAL
					TConnect reconnect;
				#endif  // COURT_APPEAL

					if (group->variant != CONTENTS_3)
					{
						if (connect.TryBy(session->key, forty1 ? CONNECT_TYPE_LOWER_INSTANCE :
							CONNECT_TYPE_FLOOR_LAWSUIT))
						{
							session->connect << connect;
						#if COURT_TYPE == COURT_APPEAL
							if (forty1 && reconnect.TryBy(session->key, CONNECT_TYPE_RECALL_LAWSUIT))
								session->subcount++;
						#endif  // APPEAL
						}
					}

					session->no = x + 1;
					office.Emit(tr1start, tr1final, session->emptyRow ? other_fields : pls_fields, session);

				#if COURT_TYPE == COURT_APPEAL
					if (session->subcount == 2)
					{
						session->connect << reconnect;
						office.Emit(tr1start, tr1no, null_fields, NULL);
						office.Emit(tr1lower, tr1final, session->emptyRow ? other_fields : pls_fields, session);
					}
				#endif  // APPEAL
				}
			}

			office.Emit(tr1final, hd1final, null_fields, NULL);
			wait.TickCount(fullCount - sessionList.Count(), "обработени заседания");
		}

		office.Emit(hd1final, NULL, null_fields, NULL);
		office.Show(NULL);
	}
}

#include "SessionListDialog.h"
#include "SessionListDialog.cpp"

void session_list(TWindow *parent)
{
	TPrintSessionGroup group;
	TSessionListDialog(parent, &group).Execute();
}
