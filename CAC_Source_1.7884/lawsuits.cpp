#include "cac.h"

// 2010:152 LPR/LRQ: unified
static void change_future_judge(TLawsuit *lawsuit)
{
	TUser user;
	const char *judge = lawsuit->judge;

	strcpy(user.ucn, judge);
	// 2010:152 LPR/LRQ: inactive judges will appear empty on open result
	if (user.flags & FLAG_ACTIVE)
	{
		TSession session;
		msql m(session.Name);

		m.Add(lawsuit, "F_NO, F_YEAR, F_TYPE");
		m.Add("F_DATE");
		m.printf(">= %ld", Today.Value());
		m.AddChars("F_KIND", KIND_OPEN_SESSIONS);
		m.AddChar("F_RESULT", RESULT_FIXED);
		m.AddString("NOT F_JUDGE", judge);

		TQuery q(str(m));
		while (q.Read())
		{
			session << q;

			// 2010:152 LPR/LRQ: sync session judges
			if (strcmp(session.president, judge) && strcmp(session.judgeX, judge) && strcmp(session.reJudgeX, judge))
			{
				if (!strcmp(session.president, session.judge))
					strcpy(session.president, judge);
				else if (!strcmp(session.judgeX, session.judge))
					strcpy(session.judgeX, judge);
				else if (!strcmp(session.reJudgeX, session.judge))
					strcpy(session.reJudgeX, judge);
			}

			strcpy(session.judge, lawsuit->judge);
			session.Update("F_JUDGE, F_JUDGE_X, F_REJUDGE_X, F_PRESIDENT");
		}
	}
}

#if COURT_TYPE == COURT_AREAL
#include "FirmLawsuitSideListDialog.h"
#include "FirmLawsuitSideListDialog.cpp"
#endif  // COURT_AREAL

static void edit_lawinds(TWindow *parent, TLawsuit *lawsuit, long sideMode, TRequest *request)
{
#if COURT_TYPE == COURT_AREAL
	if (lawsuit->key.type == TYPE_FIRM_LAW)
	{
		TFirmLawsuitSideListDialog(parent, lawsuit, sideMode).Execute();
		return;
	}
#endif  // AREAL
	TLawsuitSideListDialog(parent, lawsuit, request, sideMode).Execute();
}

#include "ImportantDialog.h"
#include "ImportantDialog.cpp"

#include "EditLawsuitDialog.h"
#include "EditLawsuitDialog.cpp"

bool edit_lawsuit(TWindow *parent, TLawsuit *lawsuit)
{
	char status = lawsuit->Status();	// 2003:295 cache

	if (strchr(RESULT_CLEANS, status) || ask("Делото е със статус %s. Ще го редактирате ли?", Result->Seek(status)))
	{
		TGArray<TConnect> connects;
		char predefined[SIZE_OF_CONNECT_TYPES];
		char tJudge[SIZE_OF_UCN];

		load_connects(lawsuit, &connects);
		*predefined = '\0';
		// 2006:341 We support many required now, don't break on 1st
		for (int i = 0; i < connects.Count(); i++)
			if (strchr(CONNECT_TYPE_REQUIREDS, connects[i]->connectType))
				strcach(predefined, connects[i]->connectType);
		strcpy(tJudge, lawsuit->judge);

		if (TEditLawsuitDialog(parent, lawsuit, &connects, predefined).Execute() == IDOK)
		{
			if (lawsuit->key.type == TYPE_FIRM_LAW && !strcmp(tJudge, INTER) && strcmp(lawsuit->judge, INTER))
				schedule(SCHED_FREGISTER, lawsuit, &lawsuit->date, NULL, lawsuit->judge);

			log_lawsuit(LOG_EDIT, CM_CLERK_EDIT_LAWSUIT, lawsuit);

			if (strcmp(lawsuit->judge, tJudge))
				change_future_judge(lawsuit);

			if (lawsuit_blocked_limited(lawsuit))  // 2018-10-23: LPR
				eportal_delete_limited(lawsuit);
			else
				eportal_send(parent, lawsuit, LOG_WRITE);  // 2016:223

			TRequest *request = NULL;
			TRequest tRequest;

			if (lawsuit->source.type == TYPE_REQUEST)
			{
				tRequest.key = lawsuit->source;

				if (tRequest.Get())	// must read all fields
				{
					request = &tRequest;
					request->subject = lawsuit->subject;
					request->quick = lawsuit->quick & (QUICK_QUICK | QUICK_IMMEDIATE);	// 2003:262
					if (!request->Update("F_SUBJECT, F_QUICK"))
						error("Входящият документ по делото не се актуализира!");
					write_connects(request, &connects, "входящия документ");
				}
				else
					error("Няма достъп до входящия документ по делото!");
			}

			edit_lawinds(parent, lawsuit, SIDE_MODE_EDIT, request);

			// 2006:081 LPR: xfer any new sides to prove
			TProve prove;

			if (prove.TryBy(lawsuit))
			{
				TSideWind sideWind;
				copy_winds(lawsuit, &prove, sideWind);
			}

			return true;
		}
	}

	return false;
}

// 2007:016 LPR: static
static void next_set_session(TWindow *parent, TRCDKeyContainer *container)
{
	TSession session;
	TLawsuit *lawsuit = (TLawsuit *) container;

	if (lawsuit->key.type != TYPE_FIRM_LAW && !strchr(KIND_FIRST_SET_LAWS, lawsuit->kind) && can_func(RIGHT_FIX_LAW) &&
		ask("Желаете ли да насрочите делото?"))
	{
		set_session(parent, &session, lawsuit, NULL, NULL);
	}
}

#include "NewLawsuitDialog.h"
#include "NewLawsuitDialog.cpp"

void new_lawsuit(TWindow *parent, TRequest *request)
{
	TLawsuit lawsuit;

	lawsuit << request;
	lawsuit.date = Today;
	lawsuit.key.no = 0;
	lawsuit.source = request->key;
	lawsuit.sourceKind = request->kind;
	// 2007:219 LRQ: request text >> lawsuit text
	//lawsuit.text = '\0';
	lawsuit.kind = request->lawKind;	// 2011:255 pre-init for xfer

	{
		// 2007:218 LPR: import new distribution data into lawsuit
		TRequestResolution *const resolution = request->LastResolution(NULL);
		TVirtPtr VP(resolution);	// 2016:021 instead of delete

		if (resolution && atob(resolution->assign))
		{
			msql m(Composition->Name);

			m.AddString("F_JUDGE", resolution->assign);
			m.AddChar("F_COLLEGE", request->college);
			strcpy(lawsuit.judge, resolution->assign);
			lawsuit.key.no = resolution->lawChoiceNo;
			// 2007:219 LRQ: set previousDate, composition
			lawsuit.previousDate = resolution->date;

			TQuery q(str(m));
			while (q.Read())
			{
				*Composition << q;

				if (strchr(Default->compositions, Composition->composition))
				{
					lawsuit.composition = Composition->composition;
					break;
				}
			}
		}
	#if COURT_TYPE == COURT_AREAL
		// 2007:218 LPR: judge init moved outside dialog
		else if (strchr(KIND_FIRM_INDOCS, request->kind))
			strcpy(lawsuit.judge, INTER);
	#endif  // AREAL
		lawsuit.subject = request->subject;	// 2007:219 LPR
	}

	if (!lawsuit.subject)
		lawsuit.subject = suggest_lawsuit_subject(request->kind);

	lawsuit.kind = request->lawKind;
	lawsuit.key.type = kind2type(lawsuit.kind);

	// 2006:310
	TGArray<TConnect> connects;
	char lawKinds[2] = { lawsuit.kind, '\0' };
	char sourceKinds[2] = { lawsuit.sourceKind, '\0' };

	load_connects(request, &connects);
	for (int i = 0; i < connects.Count(); i++)
		connects[i]->key = lawsuit.key;

	if (TNewLawsuitDialog(parent, &lawsuit, &connects, lawKinds, sourceKinds, NULL, request).Execute() == IDOK)
	{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_REGIONAL
		log_lawsuit(LOG_NEW_LAW, CM_CLERK_NEW, &lawsuit);
#endif  // APPEAL || REGIONAL
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
		log_lawsuit(LOG_NEW_LAW, CM_CLERK_NEW_1ST, &lawsuit);
#endif  // ADMIN || AREAL || MILITARY
		subvert_source(request, &lawsuit);
		eportal_send(parent, &lawsuit, LOG_WRITE);	// 2016:223

		// 2006:310 LPR: completely replaced, transfers quick too
		request->subject = lawsuit.subject;
		request->quick = lawsuit.quick & (QUICK_QUICK | QUICK_IMMEDIATE);		// 2003:262
		if (!request->Update("F_SUBJECT, F_QUICK"))
			error("Входящият документ по делото не се актуализира!");
		write_connects(request, &connects, "входящия документ");

		// 2005:341
		TProve prove;

		if (prove.TryBy(request))
		{
			prove.lawsuit = lawsuit.key;
			if (!prove.Update("F_LAWSUIT_NO, F_LAWSUIT_YEAR, F_LAWSUIT_TYPE"))
				error("Доказателството по входящия документ не се актуализира!");
		}

		// 2009:163
		TReplacePairs repairs;
		repairs.key = lawsuit.key;
		if (repairs.Try())
		{
			repairs.key = lawsuit.key;
			repairs.Insert();
		}

		edit_lawinds(parent, &lawsuit, SIDE_MODE_ADD, request);
		chain_action(parent, &lawsuit, next_set_session);
	}
}

void next_new_lawsuit(TWindow *parent, TRCDKeyContainer *container)
{
	if (can_func(RIGHT_NEW_LAW) && ask("Желаете ли да образувате документа в дело?"))
		new_lawsuit(parent, (TRequest *) container);
}

void new_virtual(TWindow *parent, const char *predefined)
{
	TLawsuit lawsuit;
	TGArray<TConnect> connects;
	char lawKinds[SIZE_OF_KINDS];		// 2006:327 +related

	// 2007:221 LPR: no input maxLawKinds, calculated by predefined
	adjust_lawsuit_kinds(predefined, lawKinds);
	// 2007:218 LPR: no input sourceKinds, unified sender scheme
	//if (!sourceKinds)
	const char *sourceKinds = adjust_source_kinds(*lawKinds, predefined);

	if (TNewLawsuitDialog(parent, &lawsuit, &connects, lawKinds, sourceKinds, predefined, NULL).Execute() == IDOK)
	{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_REGIONAL
		log_lawsuit(LOG_NEW_LAW, CM_CLERK_NEW, &lawsuit);
#endif  // APPEAL || REGIONAL
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
		log_lawsuit(LOG_NEW_LAW, CM_CLERK_NEW_1ST, &lawsuit);
#endif  // ADMIN || AREAL || MILITARY
		edit_lawinds(parent, &lawsuit, SIDE_MODE_ADD, NULL);
		chain_action(parent, &lawsuit, next_set_session);
	}
}

bool destroy_lawsuit(TLawsuit *)
{
	// 2006:012 LRQ: don't really delete anything
	return true;
}

void enter_reregister_date(TWindow *parent, TDamn *damn, TLawsuit *lawsuit)
{
	TSubpoena subpoena;
	msql m(subpoena.Name, "F_NO");

	m.Add(damn, "F_NO, F_YEAR, F_TYPE");
	m.AddChar("F_SUBPOENA_KIND", SUBPOENA_KIND_REREGISTER);
	TQuery q(str(m));

	if (q.Read() || ask("По делото няма нито едно връчено УАС за пререгистрация. Желаете ли да продължите?"))
		edit_ucn(parent, damn, lawsuit, true, false);
}

void divert_source(TLawsuit *lawsuit, CDate *sourceDate)
{
	TRequest request;
	CDate targetDate = lawsuit->date;

	lawsuit->date = *sourceDate;
	revert_source(lawsuit);
	lawsuit->date = targetDate;
	request.key = lawsuit->source;
	subvert_source(&request, lawsuit);
}

void finished_judge(TLawsuit *lawsuit, TSession *session)
{
	strcpy(lawsuit->judge, session->judge);

	if (lawsuit->Update("F_JUDGE"))
	{
		xfer_bank_case(lawsuit, NULL, NULL, XFER_EDIT);		// 2011:334
		change_future_judge(lawsuit);
	}
}

bool renumber_lawsuit(TWindow *parent, TLawsuit *source)
{
	TLawsuit target;
	target << source;

	if (check_subpoenas(source) && TRenumberLawDialog(parent, &target).Execute() == IDOK)
	{
		renumber_law(source, &target);
		return true;
	}

	return false;
}
