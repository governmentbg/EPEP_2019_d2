#include "cac.h"

static const char *rekindable(TRCDKeyGroup *group, char)
{
	TLawsuit *lawsuit = (TLawsuit *) group;

	if (lawsuit->source.type != TYPE_REQUEST)
		return "Делото не е образувано по входящ документ";

	if (lawsuit->quick & QUICK_QUICK)
		return "Делото е бързо или незабавно производство";

	if (any(lawsuit->eispp_nmr))
		return "Делото има номер по ЕИСПП";

	TDecision decision;
	TSession session;
	msql m;

	decision.key = lawsuit->key;
	decision.FloatCriteria(m, "F_NO");
	TQuery q(str(m));
	if (q.Read())
		return NULL;

	if (!lawsuit->finished.Empty())
		return "Делото е приключило, но по него няма съдебни актове";

	session.key = lawsuit->key;
	session.FloatCriteria(m, "F_NO");
	m.AddChar("NOT F_RESULT", RESULT_FIXED);
	TQuery r(str(m));
	return r.Read() ? NULL : "По делото няма проведени заседания";
}

struct TReKindTypeDocs
{
	char type;
	const char *kinds;
};

static TReKindTypeDocs ReKindTypeDocs[] =
{
#if COURT_TYPE == COURT_APPEAL
	{ TYPE_PUNISHMENT_LAW, KIND_PUNISHMENT_INDOCS },
	{ TYPE_CITIZEN_LAW,    KIND_CITIZEN_INDOCS },
	{ TYPE_RECITIZEN_LAW,  KIND_RECITIZEN_INDOCS },
	{ TYPE_TRADE_LAW,      KIND_TRADE_INDOCS },
	{ TYPE_RETRADE_LAW,    KIND_RETRADE_INDOCS },
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	{ TYPE_ADMIN_LAW,  KIND_1ST_ADMIN_INDOCS },
	{ TYPE_ADMIN_LAW,  KIND_2ND_ADMIN_INDOCS },
	{ TYPE_TRICKY_LAW, KIND_1ST_TRICKY_INDOCS },
	{ TYPE_TRICKY_LAW, KIND_2ND_TRICKY_INDOCS },
#endif  // APPEAL
#if COURT_TYPE == COURT_AREAL
	{ TYPE_PUNISHMENT_LAW, KIND_1ST_PUNISHMENT_INDOCS },
	{ TYPE_PUNISHMENT_LAW, KIND_2ND_PUNISHMENT_INDOCS },
	{ TYPE_CITIZEN_LAW,    KIND_1ST_CITIZEN_INDOCS },
	{ TYPE_CITIZEN_LAW,    KIND_2ND_CITIZEN_INDOCS },
	{ TYPE_TRADE_LAW,      KIND_1ST_TRADE_INDOCS },
	{ TYPE_TRADE_LAW,      KIND_2ND_TRADE_INDOCS },
	{ TYPE_ADMIN_LAW,      KIND_1ST_ADMIN_INDOCS },
	{ TYPE_ADMIN_LAW,      KIND_2ND_ADMIN_INDOCS },
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
	{ TYPE_PUNISHMENT_LAW, KIND_1ST_PUNISHMENT_INDOCS },
	{ TYPE_PUNISHMENT_LAW, KIND_2ND_PUNISHMENT_INDOCS },
	{ TYPE_CITIZEN_LAW,    KIND_1ST_CITIZEN_INDOCS },
	{ TYPE_CITIZEN_LAW,    KIND_2ND_CITIZEN_INDOCS },
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	{ TYPE_MARRIAGE_LAW,   KIND_MARRIAGE_INDOCS },
	{ TYPE_CITIZEN_LAW,    KIND_CITIZEN_INDOCS },
	{ TYPE_PUNISHMENT_LAW, KIND_PUNISHMENT_INDOCS },
#endif  // REGIONAL
	{ '\0', NULL }
};

static void rekind(TGroup *group, TOriginContainer *container)
{
	if (container->kind != container->oldKind)
	{
		msql m(group->Name, MQ_UPDATE);

		m.printf("F_KIND = '%c'", container->kind);
		m.AddLong("F_NO", container->key.no);
		m.AddLong("F_YEAR", container->key.year);
		m.AddChar("F_TYPE", container->key.type);
		m.AddChar("F_KIND", container->oldKind);
		group->DataBase->Execute(str(m), ET_ERROR);
	}
}

static void rekind(TGroup *group, TRequest *request, TLawsuit *lawsuit)
{
	rekind(group, request);
	rekind(group, lawsuit);
}

#include "GetRekindTargetDialog.h"
#include "GetRekindTargetDialog.cpp"

void rekind_lawsuit(TWindow *parent)
{
	TLawsuit lawsuit;
	TRequest request;

	if (get_group(parent, &lawsuit, TYPE_REKINDABLE_LAWS, NULL, rekindable))
	{
		request.key = lawsuit.source;

		if (request.Get())
		{
			request.oldKind = request.kind;
			lawsuit.oldKind = lawsuit.kind;

			if (TGetRekindTargetDialog(parent, &request, &lawsuit).Execute() == IDOK)
			{
				TExpense expense;
				TSideWind sideWind;
				TSubpoena subpoena;
				TLinkWind linkWind;
				TSched sched;

				log(LOG_CHANGE, 0, &lawsuit.key, "на вида на дело");

				rekind(&expense, &request, &lawsuit);
				rekind(&sideWind, &request, &lawsuit);
				rekind(&subpoena, &request, &lawsuit);
				rekind(&linkWind, &request, &lawsuit);
				rekind(&sched, &request, &lawsuit);

				electric_rename(&request);
				electric_rename(&lawsuit);

				request.Update();
				// 2017:139 TRQ/FIX: was missing
				lawsuit.sourceKind = request.kind;
				lawsuit.Update();
				info("Промяната приключи.");
			}
		}
	}
}
