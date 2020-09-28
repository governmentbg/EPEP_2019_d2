#include "cac.h"

const char *match_null(TRCDKeyGroup *, const char)
{
	return NULL;
}

int repeat_func = 0;
TRCDKeyGroup *_primary_group = NULL;
char PrimaryTitle[0x100] = { '\0' };
int PrimaryCommand = 0;
const char *wrong_status = "Статусът не съответства";

void compute_kinds(const char type, const char *kindsMax, char *kinds)
{
	mstr tKinds;
	add_event_kinds(type, tKinds);

	*kinds = '\0';
	for (const char *s = str(tKinds); *s; s++)
		if (strchr(kindsMax, *s))
			strcach(kinds, *s);
}

void SetPrimaryGroup(TRCDKeyGroup *primary_group)
{
	delete _primary_group;
	_primary_group = primary_group;
}

class TGetTypesKinds
{
protected:
	TGetTypesKinds(const char *tTypes, const char *kinds, int getWhat);

	char types[SIZE_OF_TYPES];
	char kinds[SIZE_OF_KINDS];

	int resWhat;
	bool write;
	bool archeck;
};

TGetTypesKinds::TGetTypesKinds(const char *tTypes, const char *tKinds, int getWhat)
	: resWhat(IDD_GET_BASIC), write(getWhat != GET_GROUP_QUERY)
{
	archeck = write && getWhat != GET_GROUP_ARCHFN;
	build_ctypes(types, tTypes, write);

	if (getWhat == GET_GROUP_EMPTY && !strchr(types, TYPE_EMPTY))
		strcach(types, TYPE_EMPTY);

	strcpy(kinds, tKinds ? tKinds : "");

	if (getWhat == GET_GROUP_QUERY)
	{
		resWhat = IDD_GET_QUERY;
#if RANGECHECK
		if (tKinds)
			fatal("TGetTypesKinds: kinds + query");
#endif  // RANGECHECK
	}
	else
	{
		if (getWhat != GET_GROUP_EXACT)
		{
			if (strchr(types, TYPE_SURROUND) && !strchr(kinds, KIND_V_SURROUND))
				strcach(kinds, KIND_V_SURROUND);
			if (strchr(types, TYPE_PROVE) && !strchr(kinds, KIND_PROVE) && !strchr(kinds, KIND_PROVE_ACTION))
				strcach(kinds, KIND_PROVE);
			// 2009:190 LPR: getWhat nonzero -> getWhat == GET_GROUP_BASIC to avoid ambiguity
			if (any(kinds) && getWhat == GET_GROUP_BASIC)
				strcach(kinds, KIND_V_EMPTY);
		}

		char tmpKinds[SIZE_OF_KINDS];

		for (const char *type = types; *type; type++)
		{
			compute_kinds(*type, kinds, tmpKinds);

			const char *kind;

			for (kind = tmpKinds; *kind; kind++)
			{
				if (kind - tmpKinds > 1 || !strchr(KIND_V_NODATES, *kind))
				{
					resWhat = IDD_GET_GROUP;
					break;
				}
			}

			if (*kind)
				break;
		}
	}
}

#include "GetGroupDialog.h"
#include "GetGroupDialog.cpp"

TRCDKeyGroup *get_group(TWindow *parent, TRCDKeyGroup *group, const char *types, const char *kinds, const char *statuses,
	match_func match, int getWhat)
{
	TDummyContainer tGroup;
	TRCDKeyGroup *grouptr;
#if RANGECHECK
	if (statuses && !any(statuses))
		fatal("get group empty statuses");
#endif  // RANGECHECK
	if (TGetGroupDialog(parent, group ? group : &tGroup, group ? NULL : &grouptr, types, kinds, statuses, match,
		getWhat).Execute() != IDOK)
	{
		return NULL;
	}

	return group ? group : grouptr;
}

bool get_group(TWindow *parent, TRCDKeyGroup *group, const char *types, const char *kinds, const char *statuses)
{
#if RANGECHECK
	if (!group)
		fatal("get group NULL group");
#endif  // RANGECHECK
	return get_group(parent, group, types, kinds, statuses, match_null, 0) != NULL;
}

bool get_group(TWindow *parent, TRCDKeyGroup *group, const char *types, const char *kinds)
{
	return get_group(parent, group, types, kinds, (const char *) NULL);
}

TRCDKeyGroup *get_group(TWindow *parent, TRCDKeyGroup *group, const char *types, const char *kinds, match_func match,
	int getWhat)
{
	return get_group(parent, group, types, kinds, NULL, match, getWhat);
}

#include "GetSessionResolutionDialog.h"
#include "GetSessionResolutionDialog.cpp"

TRCDKeyContainer *get_session_resolution(TWindow *parent, TSession *session, const char *resolutions, const char *label,
	bool sessionSelected)
{
	TRCDKeyContainer *resolutionPtr;
	return TGetSessionResolutionDialog(parent, session, RESULT_FIXEDS, &resolutionPtr, resolutions, label,
		sessionSelected).Execute() == IDOK ? resolutionPtr : NULL;
}

#include "QueryArchiveDialog.h"
#include "QueryArchiveDialog.cpp"

void query_archive(TWindow *parent)
{
	TQueryArchiveDialog(parent, IDD_QUERY_ARCHIVE).Execute();
}

#if COURT_TYPE == COURT_ADMIN
class TCreateRequest : public TRCDKeyContainer
{
public:
	TCreateRequest();

	char college;
	CTime time;
	char lawKind;
};

TCreateRequest::TCreateRequest()
	: TRCDKeyContainer("T_REQUEST", NULL)
{
	Add(
		new TCharAlias("F_COLLEGE", &college),
		new TTime("F_TIME", &time),
		new TCharAlias("F_LAW_KIND", &lawKind),
		NULL
	);

	descs = Descs + TABLE_REQUEST;
}

// 2009:280 moved here from bloat
#include "GetCreateRequestDialog.h"
#include "GetCreateRequestDialog.cpp"

bool get_create_request(TWindow *parent, TRequest *request)
{
	return TGetCreateRequestDialog(parent, request).Execute() == IDOK;
}
#endif  // ADMIN

static bool find_eportal_side(TEPortAssign *assign, const char *involvements, const TSideWind *sideWind,
	const TLawsuit &lawsuit)
{
	TSideWind tSideWind;
	msql m(tSideWind.Name, "DISTINCT F_INVOLVEMENT");

	m.Add(&lawsuit, "F_NO, F_YEAR, F_TYPE");
	m.Add(sideWind, "F_UCN, F_UCN_TYPE");

	TQuery q(str(m));
	bool found = false;
	int matching = 0;

	while (q.Read())
	{
		tSideWind << q;
		found = true;

		if (assign->involvement)
		{
			if (tSideWind.involvement == assign->involvement)
				return true;
		}
		else if (strchr(involvements, tSideWind.involvement))
			matching++;
	}

	const char *status = NULL;

	if (!found)
		status = "Лицето не фигурира в основния списък страни по делото, нито участва в заседанията или съдебните актове";
	else if (!matching)
		status = "Лицето е страна по делото, но участието му е неподходящо";
	else if (matching >= 2)
		status =  "Лицето е страна по делото с две или повече подходящи участия. Изберете точното участие";
	else
		assign->involvement = tSideWind.involvement;

	return status ? error("%s.", status) : true;
}

#include "GetEPortalAssignDialog.h"
#include "GetEPortalAssignDialog.cpp"

bool get_eportal_assign(TWindow* parent, TEPortAssign *assign, long eportalMode, const TSideWind *sideWind)
{
	return TGetEPortalAssignDialog(parent, assign, eportalMode, sideWind).Execute() == IDOK;
}

#include "GetMoveJudicialActDialog.h"
#include "GetMoveJudicialActDialog.cpp"

bool get_move_judicial_act(TWindow* parent, TDecision &source, TSession &session)
{
	return TGetMoveJudicialActDialog(parent, source, session).Execute() == IDOK;
}

class TGetWhatExtern
{
protected:
	TGetWhatExtern(sign tEdit, bool tForceExcess);

	sign edit;
	bool forceExcess;
	int resWhat;
};

TGetWhatExtern::TGetWhatExtern(sign tEdit, bool tForceExcess)
	: edit(tEdit), forceExcess(tForceExcess),
		resWhat(forceExcess || (can_extern_sess() && tEdit == -1) ? IDD_GET_JUROR_DISTRIB : IDD_GET_GROUP)
{
}

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL

#include "GetJurorDistribDialog.h"
#include "GetJurorDistribDialog.cpp"

TBaseSession *get_juror_distrib(TWindow* parent, sign edit, const TExternSession *suggest, bool forceExcess)
{
	TBaseSession *sessptr;
	return TGetJurorDistribDialog(parent, &sessptr, edit, suggest, forceExcess).Execute() == IDOK ? sessptr : NULL;
}
#endif  // AREAL || MILITARY || REGIONAL

#include "GetExternContainerDialog.h"
#include "GetExternContainerDialog.cpp"

TExternContainer *get_extern_container(TWindow* parent, match_func match, const TExternSession *suggest, bool forceExcess,
	int getWhat)
{
	TRCDKeyGroup *grouptr;

	if (!can_extern_sess())
		grouptr = get_group(parent, NULL, TYPE_LAWSUITS, NULL, match, getWhat);
	else if (TGetExternContainerDialog(parent, &grouptr, match, suggest, forceExcess, getWhat).Execute() != IDOK)
		grouptr = NULL;

	return (TExternContainer *) grouptr;
}

#include "GetClosedLawsuitDialog.h"
#include "GetClosedLawsuitDialog.cpp"

TExternContainer *get_closed_lawsuit(TWindow* parent, CDate *sessionDate, const TExternSession *suggest, bool forceExcess)
{
	TRCDKeyGroup *grouptr;

	if (TGetClosedLawsuitDialog(parent, &grouptr, sessionDate, suggest, forceExcess).Execute() != IDOK)
		grouptr = NULL;

	return (TExternContainer *) grouptr;
}
