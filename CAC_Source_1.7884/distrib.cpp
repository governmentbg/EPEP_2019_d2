#include "cac.h"

// 2007:186 LPR: twin engine: redirect firm lawsuits to _ distribution

// ----- TAssignment ---------------------------------------------------------
class TAssignment : public TRequestResolution
{
public:
	TAssignment();

	void Init(TRequest *request);
	void Init(TRequestResolution *resolution, TRequest *request);
	uint32 Ident();
	char *Judge() { return resolution == RESOLUTION_ASSIGNED ? assign : judge; }
	bool JudCmp() { return strcmp(Judge(), resolution == RESOLUTION_ASSIGNED ? orig.assign : orig.judge); }
	void Assign(const char *kinds = NULL, TStringArray *notifieds = NULL);

	long subject;
	char lawType;
	char lawKind;

	mstr left;
	mstr right;

	// prepare/perform
	struct
	{
		CDate date;
		long subject;
		char judge[SIZE_OF_UCN];
		char assign[SIZE_OF_UCN];
		char resolution;
	} orig;

	// more distribution stuff
	char autoAssign[SIZE_OF_UCN];	// check for changed auto-assigned
	char notifyName[SIZE_OF_SHORT_BASE_NAME];	// send notification

	// 2007:247 LRQ: for display
	char indocKind;
	CDate indocDate;
	char indocText[SIZE_OF_ORIGIN_TEXT];	// 2007:337 LRQ: show

	bool operator<=(const TAssignment &that) const;

protected:
	void InitCommon(TRequest *request);
	void Log(char gop, long opc);
};

TAssignment::TAssignment()
{
	Add(
		new TLongAlias("F_SUBJECT", &subject),
		new TCharAlias("F_LAW_KIND", &lawKind),
		// 2011:333 LPF: for << >> editing
		new TCharAlias("F_LAW_TYPE", &lawType),
		new TCharAlias("F_INDOC_KIND", &indocKind),
		new TDate("F_INDOC_DATE", &indocDate, false),
		new TString("F_INDOC_TEXT", indocText, sizeof indocText),
		NULL
	);
}

void TAssignment::Init(TRequest *request)
{
	*this << request;
	resolution = RESOLUTION_NEW;
	InitCommon(request);
	kind = KIND_REQUEST_RESOLUTION;
	orig.date.Clear();
}

void TAssignment::Init(TRequestResolution *resolution, TRequest *request)
{
	if (strchr(RESOLUTION_PREPASSIGNS, resolution->resolution))
	{
		*this << resolution;
		InitCommon(request);
		text.cpy(request->text);
		strcpy(orig.judge, judge);
		strcpy(orig.assign, assign);
		if (DISTRIB_NOTIFYING && resolution->resolution == RESOLUTION_PREPARED &&
			resolution->text.len() < sizeof notifyName)
		{
			strcpy(notifyName, str(resolution->text));
		}
		subject = request->subject;
		orig.date = date;	// 2007:208 was in InitCommon()
	}
	else
		Init(request);
}

void TAssignment::InitCommon(TRequest *request)
{
	TGArray<TSideWind> sideWinds;
	char *categories = load_sidebasic(this, &sideWinds);

	full_side_list(left, &sideWinds, categories, SIDEWIND_LEFT);
	full_side_list(any(left) ? right : left, &sideWinds, categories, SIDEWIND_RIGHT);
	if (any(right))
		mbk_insert(&right, "с/у ", 0, 4);

	delete[] categories;

	Kind->Seek(request->kind);
	lawKind = Kind->lawKind;
	Kind->Seek(lawKind);
	lawType = Kind->type;

	orig.subject = subject;
	orig.resolution = resolution;

	indocKind = request->kind;
	indocDate = request->date;
	strcpy(indocText, request->text);
}

void TAssignment::Log(char gop, long opc)
{
	mstr m;

	m.cat(Resolution->Seek(resolution));

	if (atob(assign))
		m.printf(", р%s", assign);
	if (atob(autoAssign))
		m.printf(", а%s", autoAssign);

	log(gop, opc, this, str(m));
}

void TAssignment::Assign(const char *kinds, TStringArray *notifieds)
{
	// pass 1: save resolution
	if (orig.date != Today || JudCmp() || orig.resolution != resolution)
	{
		// 2007:257 LPR: drop to delete below on empty judge
		if (orig.date == Today && atob(judge))
		{
			Update();
			Log(LOG_EDIT, notifieds ? CM_JUDGE_DISTRIBUTE : CM_CLERK_DISTRIBUTE);
		}
		else
		{
			if (strchr(RESOLUTION_PREPASSIGNS, orig.resolution))
			{
				date = orig.date;
				delete_resolution(this);
			}
			if (atob(Judge()))
			{
				date = Today;
				Insert();
				Log(LOG_ENTER, notifieds ? CM_JUDGE_DISTRIBUTE : CM_CLERK_DISTRIBUTE);
			}
		}

		if (DISTRIB_NOTIFYING && notifieds && *notifyName)
			distrib_notify(notifyName, notifieds, kinds);
	}

	// pass 2: save request subject
	if (subject != orig.subject)
	{
		TRequest request;

		request.key = key;
		if (request.Get())
		{
			request.subject = subject;
			request.Update("F_SUBJECT");
		}
	}
}

bool TAssignment::operator<=(const TAssignment &that) const
{
	return indocDate != that.indocDate ? indocDate < that.indocDate :
		key.year != that.key.year ? key.year < that.key.year : key.no <= that.key.no;
}

// ----- TDistribution -------------------------------------------------------
class TDistribution : public TProcess
{
public:
	TDistribution(TWindow *parent, const char *kinds, TGArray<TSubject> *tSubjects, bool calcLoad, long balance);
	virtual ~TDistribution();

	void Distribute(TAssignment *assignment);

	TGArray<TUser> users;
	TGArray<TSubject> subjects;
	TGArray<TUserSubject> dists;

	const char *DistribLog();

protected:
	char *distribLog;
};

TDistribution::TDistribution(TWindow *parent, const char *kinds, TGArray<TSubject> *tSubjects, bool calcLoad, long balance)
	: TProcess(parent, "разпределението", WAIT), distribLog(NULL)
{
	msql m;
	int i;

	dprintf("Входни предмети:");
	if (tSubjects)
		for (i = 0; i < tSubjects->Count(); i++)
			dprintf(" %c%ld", *Type->Seek((*tSubjects)[i]->type), (*tSubjects)[i]->subject);
	else if (kinds)
		dprintf(" Всички");
	else
		dprintf("Никакви");

	dprintf("\nВходни видове дела:");
	if (kinds)
		for (const char *kind = kinds; *kind; kind++)
			dprintf(" %s", Kind->Seek(*kind));
	else if (tSubjects)
		dprintf(" Всички");
	else
		dprintf(" Никакви");

	// 2009:136 FIX: discard non-judge user subjects
	m.Begin("T_USER JOIN T_USER_SUBJECT ON T_USER_SUBJECT.F_UCN = T_USER.F_UCN");
	m.AddChars("F_POST", POST_JUDGES);
	TQuery q(str(m));

	dprintf("\n\nЗаредени предмети:\n\n");
	while (q.Read())
	{
		bool matches = false;

		*UserSubject << q;
		Subject->subject = UserSubject->subject;
		Subject->type = UserSubject->type;

		if (Subject->Get() && (Subject->flags & FLAG_ACTIVE) && strmatch(Subject->kind, kinds))
		{
			if (!tSubjects)
				matches = true;
			else
			{
				for (i = 0; i < tSubjects->Count(); i++)
				{
					if ((*tSubjects)[i]->type == UserSubject->type &&
						(*tSubjects)[i]->subject == UserSubject->subject)
					{
						matches = true;
						break;
					}
				}
			}
		}

		if (matches)
		{
			strcpy(User->ucn, UserSubject->ucn);

			if (
				User->Get() &&
				(User->flags & FLAG_ACTIVE) &&
				User->date < Today
			)
			{
				TUserSubject *tUserSubject = new TUserSubject;

				*tUserSubject << UserSubject;
				dists.Add(tUserSubject);

				for (i = 0; i < subjects.Count(); i++)
					if (subjects[i]->type == UserSubject->type && subjects[i]->subject == UserSubject->subject)
						break;

				if (i == subjects.Count())
				{
					TSubject *tSubject = new TSubject;

					*tSubject << Subject;
					subjects.Add(tSubject);
					dprintf("%s, предмет %lu\n", Type->Seek(Subject->type), Subject->subject);
				}

				for (i = 0; i < users.Count(); i++)
					if (!strcmp(users[i]->ucn, UserSubject->ucn))
						break;

				if (i == users.Count())
				{
					TUser *tUser = new TUser;

					*tUser << User;
					strcpy(tUser->ucn, UserSubject->ucn);
					tUser->Get();
					if (calcLoad)
						tUser->CalcLoad(balance);
					users.Add(tUser);
				}
			}
		}
	}

	dprintf("\nЗаредени съдии:\n\n");

	for (i = 0; i < users.Count(); i++)
	{
		m.clear();
		users[i]->PrintLoad(m, true);
		dprintf("%s\n", str(m));
	}

	dprintf("\n");
	delete wait;
	wait = NULL;
}

TDistribution::~TDistribution()
{
	delete[] distribLog;
}

static bool less_loaded(TUser *last, TUser *test, char kind)
{
	if (!last)
		return true;

	int instIndex = 0;
	int typeIndex = 0;
	int kindIndex = 0;

	if (DISTRIB_LOAD_BALANCE)
	{
		Kind->Seek(kind);
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
		instIndex = !strchr(KIND_1ST_LAWSUITS, Kind->lawKind);
#endif  // ADMIN || AREAL || MILITARY
		typeIndex = strchr(TYPE_LAWSUITS, Kind->type) - TYPE_LAWSUITS;
		kindIndex = strchr(KIND_LAWSUITS, Kind->lawKind) - KIND_LAWSUITS;
	}

	if (test->FullLoad() != last->FullLoad())
		return test->FullLoad() < last->FullLoad();

	if ((DISTRIB_LOAD_BALANCE & LOAD_BALANCE_INSTANCE) &&
		last->instLoad[instIndex] != test->instLoad[instIndex])
		return test->instLoad[instIndex] < last->instLoad[instIndex];

	if ((DISTRIB_LOAD_BALANCE & LOAD_BALANCE_TYPE) &&
		last->typeLoad[typeIndex] != test->typeLoad[typeIndex])
		return test->typeLoad[typeIndex] < last->typeLoad[typeIndex];

	if ((DISTRIB_LOAD_BALANCE & LOAD_BALANCE_KIND) &&
		last->kindLoad[kindIndex] != test->kindLoad[kindIndex])
		return test->typeLoad[kindIndex] < last->typeLoad[kindIndex];

	// 2014:345 was return false
	srand(atoi(last->ucn) + atoi(test->ucn) + last->rawload + test->rawload);
	return rand() < rand();
}

void TDistribution::Distribute(TAssignment *assignment)
{
	TUser *last = NULL;

	dprintf("\nПРЕПИСКА %ld/%ldг., предмет %ld\n\n", assignment->key.no, assignment->key.year, assignment->subject);
	for (int j = 0; j < dists.Count(); j++)
	{
		if (dists[j]->subject == assignment->subject)
		{
			mstr m;
			int i;

			for (i = 0; i < users.Count(); i++)
				if (!strcmp(dists[j]->ucn, users[i]->ucn))
					break;

			users[i]->PrintLoad(m, true);
			dprintf("%s\n", str(m));

			if (less_loaded(last, users[i], assignment->kind))
				last = users[i];
		}
	}

	if (last)
	{
		char name[SIZE_OF_USER_NAME];

		dprintf("\n");
		print_user(name, last->ucn);
		dprintf("Разпределена на %s\n", name);
		last->rawload++;
		strcpy(assignment->assign, last->ucn);
		strcpy(assignment->autoAssign, assignment->assign);
	}
	else
		dprintf("Не е разпределена, няма съдия за предмет %ld\n", assignment->subject);
}

const char *TDistribution::DistribLog()
{
	if ((distribLog = tf.ReadAll()) == NULL)
		distribLog = strnew("Грешка при четене на журнала от разпределението.");

	strrep(distribLog, '\n', '\v');
	return distribLog;
}

// ----- prepare -------------------------------------------------------------
class TAssignmentGroup : public TPeriodGroup
{
public:
	TAssignmentGroup();

	char kinds[SIZE_OF_LAW_KINDS];
};

TAssignmentGroup::TAssignmentGroup()
	: TPeriodGroup("T_ASSIGNMENT")
{
	Add(new TCharList("F_KINDS", kinds, sizeof kinds));
}

static void suggest_prepare_judge(TAssignment *assignment)
{
	// faster than DB search, even on a local machine (30%)
	TGArray<TUserAliasGroup> users;

	UserAliasGroup->LoadData(&users, POST_VISIBLE_JUDGES, FLAG_ACTIVE);

	for (int i = 0; i < users.Count(); i++)
	{
		if (strchr(users[i]->lawKinds, assignment->lawKind))
		{
			strcpy(assignment->judge, users[i]->ucn);
			break;
		}
	}
}

#include "GetAssignmentCriteriaDialog.h"
#include "GetAssignmentCriteriaDialog.cpp"

#define assignment OFFICE_GROUP_CAST(TAssignment)

static void assignment_no_year(mstr &m, TGroup *group) { m.printf("%ld/%ld", assignment->key.no, assignment->key.year); }

static void assignment_subject(mstr &m, TGroup *group)
{
	if (assignment->subject)
	{
		Subject->Seek(assignment->lawType, assignment->subject);
		Subject->Print(m);
	}
}

static void assignment_left(mstr &m, TGroup *group) { m.cat(assignment->left); }

static void assignment_judge(mstr &m, TGroup *group)
{
	if (atob(assignment->judge))
		print_user(m, assignment->judge);
}

static void assignment_assign(mstr &m, TGroup *group)
{
	if (atob(assignment->assign))
		print_user(m, assignment->assign);
}

static void assignment_resolution(mstr &m, TGroup *group) { m.cat(Resolution->Seek(assignment->resolution)); }

#undef assignment

#include "AssignmentListWindow.h"
#include "AssignmentListWindow.cpp"

#include "AssignmentDialog.h"
#include "AssignmentDialog.cpp"

#include "EditPrepareAssignmentDialog.h"
#include "EditPrepareAssignmentDialog.cpp"

#include "PrepareAssignmentListWindow.h"
#include "PrepareAssignmentListWindow.cpp"

#include "PrepareAssignmentDialog.h"
#include "PrepareAssignmentDialog.cpp"

static void realize_crit(TWindow *parent, TAssignmentGroup *group, TGSArray<TAssignment> *array)
{
	TWaitWindow wait(parent, WAIT);
	TRequest request;
	// 2014:210 FB2: nested select -> join
	msql m("T_REQUEST R JOIN T_KIND K ON K.F_KIND = R.F_KIND", "R.*");

	m.AddRange("R.F_DATE", group->minDate, group->maxDate);
	m.AddChars("K.F_LAW_KIND", group->kinds);

	TQuery q(str(m));
	while (q.Read())
	{
		request << q;

		if (request.Status() == RESOLUTION_NEW)
		{
			TAssignment *assignment = new TAssignment;
			assignment->Init(&request);
			array->Add(assignment);
		}
	}
}

void prepare_assignment(TWindow *parent)
{
	TAssignmentGroup group;

	if (TGetAssignmentCriteriaDialog(parent, &group).Execute() == IDOK)
	{
		mstr m;

		m.printf("подготовка: %s", group.kinds);
		log(LOG_DISTRIB, CM_CLERK_DISTRIBUTE, str(m));

#if COURT_TYPE == COURT_AREAL
		if (!strcmp(group.kinds, KIND_FIRM_LAWS))
		{
			begNEndNYearTypesAC inpDiaryDlgData;

			inpDiaryDlgData.begDate = group.minDate;
			inpDiaryDlgData.endDate = group.maxDate;
			strcpy(inpDiaryDlgData.kinds, KIND_FIRM_LAWS);
			strcpy(inpDiaryDlgData.types, TYPE_FIRM_LAWS);
			inpDiaryDlgData.want1stInstLaws = true;
			inpDiaryDlgData.want2ndInstLaws = true;

			begMYEndMYTypeCIU plainJChkupDlgData;

			plainJChkupDlgData.begDay = 1;
			plainJChkupDlgData.endDay = Today.day;
			plainJChkupDlgData.begMonth = 1;
			plainJChkupDlgData.begYear = Today.year;
			plainJChkupDlgData.endMonth = Today.month;
			plainJChkupDlgData.endYear = Today.year;
			strcpy(plainJChkupDlgData.types, TYPE_FIRM_LAWS);

			prepare_firm_distribution(parent, inpDiaryDlgData, plainJChkupDlgData);
		}
		else
#endif  // AREAL
		{
			TGSArray<TAssignment> array;
			realize_crit(parent, &group, &array);
			TDistribution distribution(parent, group.kinds, NULL, false, false);

			if (TPrepareAssignmentDialog(parent, &array, &group, &distribution).Execute() == IDOK)
				for (int i = 0; i < array.Count(); i++)
					array[i]->Assign();
		}
	}
}

// ----- perform -------------------------------------------------------------
class TDistHistResolution : public TRequestResolution
{
public:
	TDistHistResolution();

	TLawsuit lawsuit;
};

TDistHistResolution::TDistHistResolution()
{
}

#define resolution OFFICE_GROUP_CAST(TRequestResolution)

static void disthist_assign(mstr &m, TGroup *group)
{
	print_user(m, resolution->assign);
	if (any(resolution->text))
		m.printf("\n%s", str(resolution->text));
}

#undef resolution

#define resolution OFFICE_GROUP_CAST(TDistHistResolution)

static void disthist_judge(mstr &m, TGroup *group) { print_user(m, resolution->judge); }

#define lawsuit resolution->lawsuit

static void disthist_lawsuit_kind(mstr &m, TGroup *group)
{
	if (lawsuit.kind)
		m.cat(KindAlias->Seek(lawsuit.kind));
}

static void disthist_lawsuit_no(mstr &m, TGroup *group) { keygroup_key_no(m, &lawsuit); }
static void disthist_lawsuit_year(mstr &m, TGroup *group) { keygroup_key_year(m, &lawsuit); }

#undef lawsuit

#undef resolution

static const offield disthist_fields[] =
{
	{ "F_NO",			keygroup_key_no },
	{ "F_YEAR",			keygroup_key_year },
	{ "F_REQUEST_KIND",	container_kind },
	{ "F_DATE",			container_date },
	{ "F_JUDGE",		disthist_judge },
	{ "F_ASSIGN",		disthist_assign },
	{ "F_LAWSUIT_KIND",	disthist_lawsuit_kind },
	{ "F_LAWSUIT_NO",		disthist_lawsuit_no },
	{ "F_LAWSUIT_YEAR",	disthist_lawsuit_year },
	{ NULL,			NULL }
};

class TDistHistQuery : public TFloatQuery
{
public:
	TDistHistQuery(TWindow *parent, const char *criteria, const char *title, const char *tKinds);

protected:
	const char *kinds;

	virtual void Count();
};

TDistHistQuery::TDistHistQuery(TWindow *parent, const char *criteria, const char *title, const char *tKinds)
	: TFloatQuery(parent, criteria, "DistHist.htm", title), kinds(tKinds)
{
}

void TDistHistQuery::Count()
{
	TQuery q(criteria);
	TDistHistResolution resolution;
	TRequest request;

	while (q.Read() && Increment())
	{
		resolution << q;
		request.key = resolution.key;

		if (!request.Get("F_KIND"))
		{
			count--;
			continue;
		}
		Kind->Seek(request.kind);
		if (!strchr(kinds, Kind->lawKind))
		{
			count--;
			continue;
		}
		resolution.kind = request.kind;

		TLawsuit *lawsuit = &resolution.lawsuit;
		
		if (!lawsuit->TryBy(&resolution, "F_KIND, F_NO, F_YEAR"))
		{
			lawsuit->kind = KIND_NULL;
			lawsuit->key.no = lawsuit->key.year = 0;
		}
		office.Emit(tr1start, tr1final, disthist_fields, &resolution);
	}
}

static void query_distrib(TWindow *parent, TAssignmentGroup *group)
{
	mstr title;
	TRequestResolution resolution;
	msql m(resolution.Name);

	group->AddRange(m);
	m.Add("NOT F_ASSIGN");
	m.cat("IS NULL");
	m.AddOrder("F_DATE, F_YEAR, F_NO");

	title.printf("Разпределение на преписки за %s, %D-%Dг.", Kind->Seek(*group->kinds), &group->minDate, &group->maxDate);

	TDistHistQuery(parent, str(m), str(title), group->kinds).Execute();
}

#include "EditPerformAssignmentDialog.h"
#include "EditPerformAssignmentDialog.cpp"

#include "UserLoadListBox.h"
#include "UserLoadListBox.cpp"

#include "PerformAssignmentListWindow.h"
#include "PerformAssignmentListWindow.cpp"

#include "PerformAssignmentDialog.h"
#include "PerformAssignmentDialog.cpp"

#define assignment OFFICE_GROUP_CAST(TAssignment)

static void distprot_request_kind(mstr &m, TGroup *group) { m.cpy(Kind->Seek(assignment->indocKind)); }
static void distprot_lawsuit_kind(mstr &m, TGroup *group) { m.cpy(Kind->Seek(assignment->lawKind)); }

#undef assignment

#define distrib OFFICE_GROUP_CAST(TDistribution)

static void distprot_log(mstr &m, TGroup *group)
{
	m.cat(distrib->DistribLog());
}

#undef distrib

static const offield distprot_fields[] =
{
	{ "F_NOW",			generic_now },
	{ "F_USER",			generic_login_name },
	{ "F_NO",			keygroup_key_no },
	{ "F_YEAR",			keygroup_key_year },
	{ "F_REQUEST_KIND",	distprot_request_kind },
	{ "F_ASSIGN",		disthist_assign },
	{ "F_LAWSUIT_KIND",	distprot_lawsuit_kind },
	{ "F_DISTRIB_LOG",	distprot_log },
	{ NULL,			NULL }
};

static void distrib_protocol(TWindow *parent, TGSArray<TAssignment> *array, TDistribution *distrib)
{
	TOffice office;
	TWaitWindow wait(parent, WAIT);

	if (office.Open("DistProt.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, distprot_fields, NULL);
		for (int i = 0; i < array->Count(); i++)
			office.Emit(tr1start, tr1final, distprot_fields, (*array)[i]);
		office.Emit(tr1final, NULL, distprot_fields, distrib);

		const char *dir = Electrics[TYPE_TEXT].docketPath;

		if (exist(dir, false))
		{
			char name[SIZE_OF_PATH];
			char tName[SIZE_OF_PATH];

			build_fn(tempdir, "DistProt.htm", name);
			for (int i = 0; i < array->Count(); i++)
			{
				electric_name((*array)[i], dir, tName, TYPE_TEXT);
				copy_overead_file(name, tName, false, true);
			}
		}

		office.Show(&wait);
	}
}

static bool perform_assignment(TWindow *parent, TGSArray<TAssignment> *array, char kind)
{
#if COURT_TYPE == COURT_AREAL
	if (kind == KIND_FIRM_LAW)
	{
		begNEndNYearTypesAC inpDiaryDlgData;
		begMYEndMYTypeCIU plainJChkupDlgData;

		strcpy(inpDiaryDlgData.kinds, KIND_FIRM_LAWS);
		strcpy(plainJChkupDlgData.types, TYPE_FIRM_LAWS);

		perform_firm_distribution(parent, inpDiaryDlgData, plainJChkupDlgData);
	}
	else
#endif  // AREAL
	if (array->Count() || ask("Няма преписки за образуване в %s. Желаете ли да продължите?", Kind->Seek(kind)))
	{
		parent->ShowWindow(SW_HIDE);
		constant tKinds = C2S[kind];
		TDistribution distribution(parent, tKinds, NULL, true, DISTRIB_LOAD_BALANCE != 0);

		log(LOG_DISTRIB, CM_JUDGE_DISTRIBUTE, 0, 0, kind2type(kind), kind, NULL);

		if (TPerformAssignmentDialog(parent, array, &distribution, tKinds).Execute() == IDOK)
		{
			TStringArray notifieds;

			for (int i = 0; i < array->Count(); i++)
				(*array)[i]->Assign(tKinds, &notifieds);

			distrib_protocol(parent, array, &distribution);
			parent->ShowWindow(SW_SHOW);
			return true;
		}

		// 2014:345 revert for re-launch
		for (int i = 0; i < array->Count(); i++)
		{
			TAssignment *assignment = (*array)[i];
			assignment->resolution = RESOLUTION_PREPARED;
			*assignment->autoAssign = *assignment->assign = '\0';
		}
	}

	parent->ShowWindow(SW_SHOW);
	return false;
}

static void realize_crit(const char *kinds, TGSArray<TAssignment> *array)
{
	msql m;
	TRequestResolution resolution;
	TRequest request;

	if (DISTRIB_UNPREPARED_REQS)
	{
		m.Begin("T_REQUEST R JOIN T_KIND K ON K.F_KIND = R.F_KIND", "R.*");

		m.Add("R.F_DATE");
		m.printf(">= %ld", WeekAgo.Value());
		m.AddChars("K.F_LAW_KIND", kinds);
		m.Add("(SELECT COUNT(F_NO) FROM T_REQUEST_RESOLUTION RR WHERE RR.F_NO = R.F_NO AND RR.F_YEAR = R.F_YEAR)");
		m.cat("= 0");

		TQuery q(str(m));
		while (q.Read())
		{
			TAssignment *assignment = new TAssignment;

			request << q;
			assignment->Init(&request);
			array->Add(assignment);
		}
	}

	m.Begin(resolution.Name);
	m.Add("F_DATE");
	m.printf(">= %ld", WeekAgo.Value());
	m.AddString("F_JUDGE", Default->ucn);
	m.AddChar("F_RESOLUTION", RESOLUTION_PREPARED);

	TQuery q(str(m));
	while (q.Read())
	{
		resolution << q;

		m.Begin(resolution.Name, "F_DATE");
		m.Add(&resolution, "F_NO, F_YEAR, F_TYPE");
		m.Add("F_DATE");
		m.printf("> %ld", resolution.date.Value());

		TQuery r(str(m));
		if (r.Read())
			continue;

		request.key = resolution.key;

		if (request.Get())
		{
			Kind->Seek(request.kind);

			if (strchr(kinds, Kind->lawKind))
			{
				TAssignment *assignment = new TAssignment;
				assignment->Init(&resolution, &request);
				array->Add(assignment);
			}
		}
	}
}

#include "LaunchPerformAssignmentDialog.h"
#include "LaunchPerformAssignmentDialog.cpp"

void perform_assignment(TWindow *parent)
{
	TGSArray<TAssignment> array[LAW_KINDS_MAX];
	const char *kinds = Default->lawKinds;

	for (int i = 0; kinds[i]; i++)
	{
#if COURT_TYPE == COURT_AREAL
		if (kinds[i] == KIND_FIRM_LAW)
			continue;
#endif  // AREAL
		realize_crit(C2S[kinds[i]], &array[i]);
	}

	// no dialog for a single kind, there is nothing to choose from
	if (strlen(kinds) == 1)
		perform_assignment(parent, &array[0], kinds[0]);
	else
		TLaunchPerformAssignmentDialog(parent, array, kinds).Execute();
}

// ----- equate --------------------------------------------------------------
static void calculate_preload(TUser *user, TGArray<TUser> *users)
{
	TTextFile tf;
	FILE *f = tf.OpenTemp("log");
	double total = 0.0;
	int found = 0;

	for (int i = 0; i < users->Count(); i++)
	{
		if (strcmp((*users)[i]->ucn, user->ucn))
		{
			mstr m;

			total += (*users)[i]->FullLoad();
			(*users)[i]->PrintLoad(m, true);
			fprintf(f, "%s\n", str(m));
		}
		else
		{
			user->rawload = (*users)[i]->rawload;
			found = 1;
		}
	}

	if (users->Count() - found)
	{
		double tAverage = total / (users->Count() - found);
		long average = round(tAverage);
		mstr m;

		fprintf(f, "\nОбщо: %.2lf, средно %.2lf -> %ld\n\n", total, tAverage, average);
		// 2007:270 LPR: assuming user rawload is already calculated
		//if (!found)
		//	user->CalcLoad(user->ucn);
		user->preload = 0;
		user->PrintLoad(m, false);
		fprintf(f, "%s\n", str(m));
		user->preload = average - user->RawLoadA();
		user->PrintLoad(m, false);
		fprintf(f, "Коригирани: %s\n", strstr(str(m), ": ") + 2);
		tf.Show(NULL);
	}
	else
	{
		info("Няма други потребители по които да се изравни натоварването.");
		user->preload = 0;
	}
}

void calculate_preload(TWindow *parent, TUser *user, TGArray<TSubject> *subjects)
{
	TDistribution distribution(parent, NULL, subjects, true, false);
	calculate_preload(user, &distribution.users);
}

static void equate_load(TGArray<TUser> *users)
{
	TTextFile tf;
	FILE *f = tf.OpenTemp();
	double total = 0.0;

	for (int i = 0; i < users->Count(); i++)
	{
		TUser *user = (*users)[i];
		mstr m;

		user->preload = 0;
		total += user->FullLoad();
		user->PrintLoad(m, true);
		fprintf(f, "%s\n", str(m));
	}

	double tAverage = total / users->Count();
	long average = round(tAverage);

	fprintf(f, "\nОбщо: %.2lf, средно %.2lf -> %ld\n\n", total, tAverage, average);

	for (int i = 0; i < users->Count(); i++)
	{
		TUser *user = (*users)[i];
		mstr m;

		user->preload = average - user->RawLoadA();
		user->PrintLoad(m, false);
		fprintf(f, "%s\n", str(m));
	}

	tf.Show(NULL);
}

class TEquateGroup : public TDummyGroup
{
public:
	TEquateGroup();

	char kinds[SIZE_OF_LAW_KINDS];
};

TEquateGroup::TEquateGroup()
{
	Add(new TCharList("F_KINDS", kinds, sizeof kinds));
}

#include "GetEquateCriteriaDialog.h"
#include "GetEquateCriteriaDialog.cpp"

#include "EquateLoadDialog.h"
#include "EquateLoadDialog.cpp"

void equate_load(TWindow *parent)
{
	TEquateGroup group;

	if (TGetEquateCriteriaDialog(parent, &group).Execute() == IDOK)
	{
		TDistribution distribution(parent, group.kinds, NULL, true, false);
		TEquateLoadDialog(parent, &distribution.users).Execute();
	}
}

// ----- distrib_user --------------------------------------------------------
// 2007:239 LRQ: non-judges mini dist
#include "UserDistLawKindsDialog.h"
#include "UserDistLawKindsDialog.cpp"

#include "UserDistributionDialog.h"
#include "UserDistributionDialog.cpp"

// 2007:149 copied from subject_corrupt(), altered
// 2007:183 subject_user() -> distrib_user()
void distrib_user(TWindow *parent, TUser *user)
{
	TGArray<TSubject> subjects, tSubjects;
	TSubject *tSubject;
	msql m;
	TGArray<TUserSubject> userSubjects;
	TUserSubject *tUserSubject;

	m.Begin(UserSubject->Name);
	m.AddString("F_UCN", user->ucn);
	TQuery r(str(m));
	while (r.Read())
	{
		tUserSubject = new TUserSubject;
		*tUserSubject << r;
		userSubjects.Add(tUserSubject);
	}

	m.Begin(Subject->Name);
	m.AddOrder("F_SUBJECT");
	TQuery q(str(m));
	while (q.Read())
	{
		*Subject << q;

		for (int i = 0; i < userSubjects.Count(); i++)
		{
			if (userSubjects[i]->subject == Subject->subject && userSubjects[i]->type == Subject->type)
			{
				tSubject = new TSubject;
				*tSubject << Subject;
				subjects.Add(tSubject);

				tSubject = new TSubject;
				*tSubject << Subject;
				tSubjects.Add(tSubject);
				break;
			}
		}
	}

	// 2007:239 LPR: mini distrib dialog for non-judges
	if (!strchr(POST_JUDGES, user->post))
		TUserDistLawKindsDialog(parent, user).Execute();
	// 2007:183 select_subjects() -> TUserDistributionDialog
	else if (TUserDistributionDialog(parent, user, &tSubjects).Execute() == IDOK)
	{
		int i = 0;

		while (i < subjects.Count())
		{
			int j;

			for (j = 0; j < tSubjects.Count(); j++)
				if (subjects[i]->subject == tSubjects[j]->subject && subjects[i]->kind == tSubjects[j]->kind)
					break;

			if (j < tSubjects.Count())
			{
				subjects.Remove(i);
				tSubjects.Remove(j);
			}
			else
				i++;
		}

		strcpy(UserSubject->ucn, user->ucn);
		for (int i = 0; i < subjects.Count(); i++)
		{
			UserSubject->subject = subjects[i]->subject;
			UserSubject->type = subjects[i]->type;
			UserSubject->Delete(true);
		}
		for (int j = 0; j < tSubjects.Count(); j++)
		{
			UserSubject->subject = tSubjects[j]->subject;
			UserSubject->type = tSubjects[j]->type;
			UserSubject->Insert();
		}
	}
}

// ----- distrib_subject -----------------------------------------------------
#include "SubjectDistributionDialog.h"
#include "SubjectDistributionDialog.cpp"

void distrib_subject(TWindow *parent, TSubject *subject, TGArray<TUserAliasGroup> *users)
{
	TSubjectDistributionDialog(parent, subject, users).Execute();
}
