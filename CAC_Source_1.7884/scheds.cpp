#include "cac.h"

#include "AddSchedDialog.h"
#include "AddSchedDialog.cpp"

#include "EditSchedDialog.h"
#include "EditSchedDialog.cpp"

#define tSched OFFICE_GROUP_CAST(TSched)

static void sched_sched(mstr &m, TGroup *group) { m.cpy(CalInterval->Seek(tSched->sched)); }

#undef tSched

#include "SchedListWindow.h"
#include "SchedListWindow.cpp"

#include "SchedListDialog.h"
#include "SchedListDialog.cpp"

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void compute_scheds(mstr &scheds, const TRCDKeyContainer *container, char originKind, char secondKind)
#endif  // AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN
void compute_scheds(mstr &scheds, const TRCDKeyContainer *container, char originKind, char)
#endif  // APPEAL || ADMIN
{
	char scheduleKind = get_schedule_kind(container);
	const TContainerSched *sched;

	for (sched = ContainerScheds; sched->originKinds; sched++)
		if (strchr(sched->originKinds, originKind) && strchr(sched->scheduleKinds, scheduleKind))
			scheds.cach(sched->sched);

	if (scheds.chr(SCHED_STOPPED))
		scheds.cach(SCHED_STOPPED_M);

	// 2002:312; 2003:064 else if -> if
	// 2010:281 sched_allow_3 -> inline
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (strchr(KIND_SESSIONS, container->kind) && container->Status() == RESULT_WITH_SETTLEMENT &&
		strchr(KIND_REQUEST_152XS, secondKind))
	{
		scheds.cach(SCHED_SETTLE_3);
	}
#endif  // AREAL || MILITARY || REGIONAL

	// 2010:281 sched allow 7 -> inline
	// 2010:281 LRQ/TRQ: fixed courts; 2011:017 -APPEAL
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	if (strchr(KIND_DECISIONS, container->kind) && strchr(KIND_1ST_PUNISHMENT_LAWS, originKind))
		scheds.cach(SCHED_DECIDE_7);
#endif   // APPEAL || AREAL || MILITARY

		// 2010:281 sched allow 15 -> inline
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	if (strchr(KIND_SETTLEMENTS, container->kind) && container->key.type == TYPE_PUNISHMENT_LAW)
		scheds.cach(SCHED_SETTLE_15);
#endif  // APPEAL || AREAL || MILITARY

	// 2006:180; 2010:281 sched allow 368 -> inline
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (strchr(KIND_SESSIONS, container->kind) && container->Status() == RESULT_WITH_SETTLEMENT &&
		originKind == KIND_PRIVATE_PUNISHMENT_LAW && secondKind == KIND_REQUEST_368)
	{
		scheds.cach(SCHED_SACTION);
	}
	// 2013:235
	if (strchr(KIND_CLOSED_SESSIONS, container->kind) &&
		(container->Status() == RESULT_SUB_SETTLEMENT || container->Status() == RESULT_WITH_ORDER) &&
		originKind == KIND_PRIVATE_PUNISHMENT_LAW)
	{
		scheds.cach(SCHED_RET_PROS_1);
	}
#endif  // AREAL || MILITARY || REGIONAL

	// 2017:236
#if COURT_TYPE == COURT_REGIONAL
	if (strchr(KIND_CLOSED_SESSIONS, container->kind))
		scheds.cach(SCHED_FILE_CLAIM);
	else if (container->kind == KIND_EXECUTIVE)
		scheds.cach(SCHED_APP_EXECOR);
#endif  // REGIONAL

	if (scheds.chr(SCHED_FIXED))
		scheds.cach(SCHED_PPREPARE);

	if (scheds.chr(SCHED_JUDGEMENT))
		scheds.cach(SCHED_JPREPARE);
}

void get_container_kinds(const TRCDKeyContainer *container, char &originKind, char &secondKind, const TRCDKey *tOrigin)
{
	TRCDKey origin;

	// block schedules
	originKind = KIND_V_LAWSUIT;
	secondKind = KIND_V_REQUEST;

	if (tOrigin)
		origin = *tOrigin;

	if (strchr(KIND_LAWSUITS, container->kind))
	{
		originKind = container->kind;
		secondKind = ((TLawsuit *) container)->sourceKind;
	}
	else if (strchr(KIND_INDOCS, container->kind))
	{
		originKind = container->kind;
		secondKind = ((TCharAlias *) container->Find("F_LAW_KIND"))->C();
	}
	else if (tOrigin || find_origin(container, origin))
	{
		if (strchr(TYPE_LAWSUITS, origin.type))
		{
			TLawsuit lawsuit;
			lawsuit.key = origin;

			if (lawsuit.Get("F_KIND, F_SOURCE_KIND"))
			{
				originKind = lawsuit.kind;
				secondKind = lawsuit.sourceKind;
			}
		}
		else if (origin.type == TYPE_REQUEST)
		{
			TRequest request;
			request.key = origin;

			if (request.Get("F_KIND, F_LAW_KIND"))
			{
				originKind = request.kind;
				secondKind = request.lawKind;
			}
		}
	}
}

void compute_scheds(mstr &scheds, const TRCDKeyContainer *container, const TRCDKey *origin)
{
	char originKind, secondKind;
	get_container_kinds(container, originKind, secondKind, origin);
	compute_scheds(scheds, container, originKind, secondKind);
}

void edit_scheds(TWindow *parent, const TRCDKeyContainer *container)
{
	TRCDKey origin;

	if (find_origin(container, origin))
	{
		mstr scheds;
		TSched sched;

		sched << container;
		sched.lawsuit = origin;
		compute_scheds(scheds, container, &origin);
		TSchedListDialog(parent, &sched, str(scheds)).Execute();
	}
}
