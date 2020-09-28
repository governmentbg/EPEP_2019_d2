#define MENU_REG		"R"
#define MENU_CLERK	"C"
#define MENU_SECR		"S"
#define MENU_JUDGE	"J"
#define MENU_ADMIN	"A"
#define MENU_ARCH		"h"
#define MENU_DELIV	"3"

const TElectra electra_globals[] =
{
	// request upper
	{ "Вписване на разпореждане", CM_CLERK_NEW_REQUEST_RESOLUTION,
		TYPE_REQUESTS, NULL,
		RESOLUTION_RESOLUTABLES, NULL,
		MENU_CLERK },
	{ "Образуване на дело", CM_CLERK_NEW_LAWSUIT,
		TYPE_REQUESTS, NULL,
		RESOLUTION_LAWABLES, request_lawable,
		MENU_CLERK },
	// 2016:013 RESOLUTION_LAWABLES -> NULL
	{ "Редактиране на входящ документ", CM_REG_EDIT,
		TYPE_REQUESTS, NULL,
		NULL, request_editable,
		MENU_REG },

	// lawsuit upper
	{ "Насрочване на заседание", CM_CLERK_SET,
		TYPE_LAWSUITS, NULL,
		NULL, lawsuit_settable,
		MENU_CLERK MENU_SECR },
	{ "Вписване на резултат от ЗЗ/РЗ", CM_CLERK_CLOSED_RESULT,
		TYPE_LAWSUITS, NULL,
		NULL, NULL,
		MENU_CLERK MENU_SECR MENU_JUDGE },
	{ "Редактиране на дело", CM_CLERK_EDIT_LAWSUIT,
		TYPE_LAWSUITS, NULL,
		NULL, lawsuit_editable,
		MENU_CLERK },

	// inreg upper
	{ "Вписване на разпореждане", CM_REG_NEW_INREG_EVENT,
		TYPE_INREGS, NULL,
		RESULT_EVENTABLES, NULL,
		MENU_REG },
	{ "Редактиране на жалби, протести", CM_REG_IN_EDIT,
		TYPE_INREGS, NULL,
		NULL, inreg_editable,
		MENU_REG },

	// outreg upper
	{ "Редактиране на изходящ документ", CM_REG_OUT_EDIT,
		TYPE_OUTREGS, NULL,
		NULL, NULL,
		MENU_REG },
	{ "Връщане на дела и документи", CM_REG_OUT_RECEIVE,
		TYPE_OUTREGS, NULL,
		NULL, NULL,
		MENU_REG },

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	// prove upper
	{ "Редактиране на веществено доказателство", CM_ARCH_EDIT_PROVE,
		TYPE_PROVES, NULL,
		NULL, NULL,
		MENU_ARCH },
	{ "Вписване на разпоредително действие", CM_ARCH_NEW_PROVE_ACTION,
		TYPE_PROVES, NULL,
		NULL, NULL,
		MENU_ARCH },
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

	// generic middle
	{ "Редактиране на срокове", CM_CLERK_EDIT_SCHEDS,
		TYPE_SCHEDULABLE_BASICS, NULL,
		NULL, schedulable,
		MENU_CLERK },
	{ "Регистриране на съпровождащ документ", CM_REG_NEW_SURROUND,
		TYPE_SURROUNDABLES, NULL,
		NULL, surroundable,
		MENU_REG },
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ "Вписване на веществено доказателство", CM_ARCH_NEW_PROVE,
		TYPE_PROVABLES, NULL,
		NULL, origin_provable,
		MENU_ARCH },
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	{ "Предоставяне/връщане", CM_CLERK_IN_OUT_MOVEMENT,
		TYPE_MOVABLES, NULL,
		NULL, NULL,
		MENU_CLERK MENU_ARCH },

	// request lower
	{ "Преномериране на входящ документ", CM_ADMIN_RENUMBER_REQUEST,
		TYPE_REQUESTS, NULL,
		RESULT_CLEANS, NULL,
		MENU_ADMIN },
	{ "Изтриване на входящ документ", CM_ADMIN_DELETE_INDOC,
		TYPE_REQUESTS, NULL,
		NULL, indoc_deletable,
		MENU_ADMIN },

	// lawsuit lower
	{ "Архивиране на дело", CM_ARCH_NEW_ARCHIVE,
		TYPE_LAWSUITS, NULL,
		NULL, extern_archable,
		MENU_ARCH },
	{ "Преномериране на дело", CM_ADMIN_RENUMBER_LAWSUIT,
		TYPE_LAWSUITS, NULL,
		RESULT_CLEANS, NULL,
		MENU_ADMIN },
	{ "Изтриване на движението на дело", CM_ADMIN_DELETE_LAWSUIT_MOVEMENT,
		TYPE_LAWSUITS, NULL,
		NULL, not_archived_killed,
		MENU_ADMIN },
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ "Изтриване на ЕИСПП номер на НП", CM_ADMIN_DELETE_EISPP_NMR,
		TYPE_PROVABLES, NULL,
		NULL, eispp_nmr_deletable,
		MENU_ADMIN },
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	{ "Изтриване на дело", CM_ADMIN_DELETE_LAWSUIT,
		TYPE_LAWSUITS, NULL,
		RESULT_CLEANS, NULL,
		MENU_ADMIN },

	// inreg lower
	{ "Преномериране на документ от входящ регистър", CM_ADMIN_RENUMBER_INREG,
		TYPE_INREGS, NULL,
		RESULT_CLEANS, NULL,
		MENU_ADMIN },
	{ "Изтриване на документ от входящ регистър", CM_ADMIN_DELETE_INDOC,
		TYPE_INREGS, NULL,
		NULL, indoc_deletable,
		MENU_ADMIN },

	// outreg lower
	{ "Изтриване на данните за връщане", CM_ADMIN_DELETE_OUTREG_RETURN,
		TYPE_OUTREGS, NULL,
		RESULT_IOREG_RETURNEDS, NULL,
		MENU_ADMIN },
	{ "Изтриване на документ от изходящ регистър", CM_ADMIN_DELETE_OUTREG,
		TYPE_OUTREGS, NULL,
		NULL, NULL,
		MENU_ADMIN },

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	// prove lower
	{ "Изтриване на веществено доказателство", CM_ADMIN_DELETE_PROVE,
		TYPE_PROVES, NULL,
		NULL, NULL,
		MENU_ADMIN },
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

	{ NULL, 0, NULL, NULL, NULL, NULL }
};

const TElectra electra_locals[] =
{
	// request resolution
	{ "Редактиране на разпореждане", CM_CLERK_EDIT_REQUEST_RESOLUTION,
		TYPE_REQUESTS, KIND_REQUEST_RESOLUTIONS,
		RESOLUTION_RESOLUTIONS,	NULL,
		MENU_CLERK },
	{ "Изтриване на разпореждане", CM_ADMIN_DELETE_REQUEST_RESOLUTION,
		TYPE_REQUESTS, KIND_REQUEST_RESOLUTIONS,
		NULL, resolution_deletable,	// 2017:202 LPR: was RESOLUTION_STANDALONES
		MENU_ADMIN },

	// open sessions
	{ "Вписване на резултат от открито заседание", CM_SECR_OPEN_RESULT,
		TYPE_LAWSUITS, KIND_OPEN_SESSIONS,
		NULL, not_aborted_present,
		MENU_SECR },
	{ "Печат на призовки/съобщения/писма/обявления", CM_CLERK_PRINT_MESSAGES,
		TYPE_LAWSUITS, KIND_OPEN_SESSIONS,
		NULL, NULL,
		MENU_CLERK MENU_SECR },
	{ "Вписване на съдебен акт", CM_CLERK_NEW_JUDICIAL_ACT,
		TYPE_LAWSUITS, KIND_OPEN_SESSIONS,
		RESULT_SURROUENTERABLES, NULL,
		MENU_CLERK MENU_SECR MENU_JUDGE },
	{ "Влизане в законна сила", CM_CLERK_FORCE_DATE,
		TYPE_FORCABLE_LAWS, KIND_OPEN_SESSIONS,
		NULL, forcable,
		MENU_CLERK MENU_SECR },
	{ "Редактиране на насрочване", CM_CLERK_EDIT_SET,
		TYPE_LAWSUITS, KIND_OPEN_SESSIONS,
		RESULT_FIXEDS, NULL,
		MENU_CLERK MENU_SECR },
	{ "Пренасрочване", CM_CLERK_RESET,
		TYPE_LAWSUITS, KIND_OPEN_SESSIONS,
		RESULT_FIXEDS, NULL,	// 2011:257 LRQ: UNSETABLES -> FIXEDS
		MENU_CLERK MENU_SECR },
	{ "Отлагане без ново насрочване", CM_CLERK_UNSET,
		TYPE_LAWSUITS, KIND_OPEN_SESSIONS,
		RESULT_FIXEDS, NULL,	// 2011:257 LRQ: UNSETABLES -> FIXEDS
		MENU_CLERK MENU_SECR },
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ "Разпределение на съдебни заседатели", CM_CLERK_NEW_DISTRIB_JURORS,
		TYPE_LAWSUITS, KIND_OPEN_SESSIONS,
		RESULT_FIXEDS, jurorable,
		MENU_CLERK MENU_SECR MENU_JUDGE },
	{ "Преразпределение на съдебни заседатели", CM_CLERK_EDIT_DISTRIB_JURORS,
		TYPE_LAWSUITS, KIND_OPEN_SESSIONS,
		RESULT_FIXEDS, jurorable,
		MENU_CLERK MENU_SECR MENU_JUDGE },
#endif  // AREAL || MILITARY || REGIONAL

	// closed sessions
	{ "Редактиране на резултат от ЗЗ/РЗ", CM_CLERK_EDIT_CLOSED,
		TYPE_LAWSUITS, KIND_CLOSED_SESSIONS,
		NULL, NULL,
		MENU_CLERK MENU_SECR MENU_JUDGE },
	{ "Печат на призовки/съобщения/писма/обявления", CM_CLERK_PRINT_MESSAGES,
		TYPE_LAWSUITS, KIND_CLOSED_SESSIONS,
		NULL, NULL,
		MENU_CLERK MENU_SECR },
	{ "Вписване на съдебен акт", CM_CLERK_NEW_JUDICIAL_ACT,
		TYPE_LAWSUITS, KIND_CLOSED_SESSIONS,
		RESULT_SURROUENTERABLES, NULL,
		MENU_CLERK MENU_SECR MENU_JUDGE },
	{ "Влизане в законна сила", CM_CLERK_FORCE_DATE,
		TYPE_FORCABLE_LAWS, KIND_CLOSED_SESSIONS,
		NULL, forcable,
		MENU_CLERK MENU_SECR },

	// any sessions
	{ "Регистриране на съпровождащ документ", CM_REG_NEW_SURROUND,
		TYPE_LAWSUITS, KIND_SESSIONS,
		NULL, surroundable,
		MENU_REG },
	{ "Изтриване на заседание", CM_ADMIN_DELETE_LAWSUIT_EVENT,
		TYPE_LAWSUITS, KIND_SESSIONS,
		NULL, NULL,
		MENU_ADMIN },

	// judicial acts
	{ "Печат на съобщения/писма/обявления", CM_SECR_PRINT_MESSAGES,
		TYPE_LAWSUITS, KIND_ENDOCS,
		NULL, NULL,
		MENU_SECR MENU_CLERK },
	{ "Влизане в законна сила", CM_SECR_FORCE_DATE,
		TYPE_FORCABLE_LAWS, KIND_FORCABLE_ENDOCS,
		NULL, NULL,
		MENU_CLERK MENU_SECR },
	{ "Редактиране на съдебен акт", CM_CLERK_EDIT_JUDICIAL_ACT,
		TYPE_LAWSUITS, KIND_ENDOCS,
		NULL, NULL,
		MENU_CLERK MENU_SECR MENU_JUDGE },
	{ "Регистриране на съпровождащ документ", CM_REG_NEW_SURROUND,
		TYPE_LAWSUITS, KIND_ENDOCS,
		NULL, surroundable,
		MENU_REG },
	{ "Изтриване на съдебен акт", CM_ADMIN_DELETE_LAWSUIT_EVENT,
		TYPE_LAWSUITS, KIND_ENDOCS,
		NULL, NULL,
		MENU_ADMIN },

	// surrounds
	{ "Редактиране на съпровождащ документ", CM_REG_EDIT_SURROUND,
		TYPE_SURROUNDS, KIND_SURROUNDS,
		NULL, NULL,
		MENU_REG },
	{ "Преномериране на съпровождащ документ", CM_ADMIN_RENUMBER_SURROUND,
		TYPE_SURROUNDS, KIND_SURROUNDS,
		RESULT_CLEANS, NULL,
		MENU_ADMIN },
	{ "Изтриване на съпровождащ документ", CM_ADMIN_DELETE_INDOC,
		TYPE_SURROUNDS, KIND_SURROUNDS,
		NULL, indoc_deletable,
		MENU_ADMIN },

	// inregs
	{ "Редактиране на жалби, протести", CM_REG_IN_EDIT,
		TYPE_INREGS, KIND_IN_REG_DOCS,
		NULL, NULL,
		MENU_REG },
	{ "Преномериране на документ от входящ регистър", CM_ADMIN_RENUMBER_INREG,
		TYPE_INREGS, KIND_IN_REG_DOCS,
		RESULT_CLEANS, NULL,
		MENU_ADMIN },
	{ "Изтриване на документ от входящ регистър", CM_ADMIN_DELETE_INDOC,
		TYPE_INREGS, KIND_IN_REG_DOCS,
		NULL, indoc_deletable,
		MENU_ADMIN },

	// inreg events
	// 2010:260 LPR/LRQ: FIX: UNDEPARTEDS -> INREG_RESOLUTIONS
	// 2016:251 LPR: INREG_RESOLUTIONS -> event_editable
	{ "Редактиране на разпореждане", CM_REG_EDIT_INREG_EVENT,
		TYPE_INREGS, KIND_INREG_USER_EVENTS,
		NULL, event_editable,
		MENU_REG },
	{ "Изтриване на разпореждане", CM_ADMIN_DELETE_INREG_EVENT,
		TYPE_INREGS, KIND_INREG_USER_EVENTS,
		NULL, event_editable,	// 2018-09-21: FIX: INREG_RESOLUTIONS -> event_editable
		MENU_ADMIN },

	// outregs
	{ "Редактиране на изходящ документ", CM_REG_OUT_EDIT,
		TYPE_OUTREGS, KIND_OUT_REG_DOCS,
		NULL, NULL,
		MENU_REG },
	{ "Връщане на дела и документи", CM_REG_OUT_RECEIVE,
		TYPE_OUTREGS, KIND_OUT_REG_DOCS,
		NULL, NULL,
		MENU_REG },
	{ "Изтриване на данните за връщане", CM_ADMIN_DELETE_OUTREG_RETURN,
		TYPE_OUTREGS, KIND_OUT_REG_DOCS,
		RESULT_IOREG_RETURNEDS, NULL,
		MENU_ADMIN },
	{ "Изтриване на документ от изходящ регистър", CM_ADMIN_DELETE_OUTREG,
		TYPE_OUTREGS, KIND_OUT_REG_DOCS,
		NULL, NULL,
		MENU_ADMIN },

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	// proves
	{ "Редактиране на веществено доказателство", CM_ARCH_EDIT_PROVE,
		TYPE_PROVES, KIND_PROVES,
		NULL, NULL,
		MENU_ARCH },
	{ "Изтриване на веществено доказателство", CM_ADMIN_DELETE_PROVE,
		TYPE_PROVES, KIND_PROVES,
		NULL, NULL,
		MENU_ADMIN },

	// prove actions
	{ "Редактиране на разпоредително действие", CM_ARCH_EDIT_PROVE_ACTION,
		TYPE_PROVES, KIND_PROVE_ACTIONS,
		NULL, NULL,
		MENU_ARCH },
	{ "Изтриване на разпоредително действие", CM_ADMIN_DELETE_PROVE_ACTION,
		TYPE_PROVES, KIND_PROVE_ACTIONS,
		NULL, NULL,
		MENU_ADMIN },
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

	// generics
	{ "Редактиране на срокове", CM_CLERK_EDIT_SCHEDS,
		TYPE_SCHEDULABLES, KIND_SCHEDULABLE_EVENTS,
		NULL, schedulable,
		MENU_CLERK },

	{ NULL, 0, NULL, NULL, NULL, NULL, NULL }
};

static void electra_back_action(TWindow *parent, int command, TRCDKeyContainer *container)
{
	TGroup *const backup = container->clone();
	TVirtPtr VP(backup);
	bool result;

	*backup << container;

	switch (command)
	{
	#define request TYPECHECK_PTR_CAST(TRequest, container)
		case CM_REG_EDIT : result = edit_request(parent, request); break;
		case CM_ADMIN_RENUMBER_REQUEST : result = renumber_request(parent, request); break;
	#undef request

	#define lawsuit TYPECHECK_PTR_CAST(TLawsuit, container)
		case CM_CLERK_EDIT_LAWSUIT : result = edit_lawsuit(parent, lawsuit); break;
		case CM_ADMIN_RENUMBER_LAWSUIT : result = renumber_lawsuit(parent, lawsuit); break;
	#undef lawsuit

	#define resolution TYPECHECK_PTR_CAST(TRequestResolution, container)
		case CM_CLERK_EDIT_REQUEST_RESOLUTION : result = edit_request_resolution(parent, resolution); break;
	#undef resolution

#define lawsuit PrimaryGroup(TLawsuit)

	#define session TYPECHECK_PTR_CAST(TSession, container)
		case CM_SECR_OPEN_RESULT : result = open_result(parent, TYPECHECK_PTR_CAST(TSession, container), lawsuit); break;
		case CM_CLERK_EDIT_CLOSED : result = edit_closed(parent, TYPECHECK_PTR_CAST(TSession, container), lawsuit); break;
		case CM_CLERK_FORCE_DATE : result = force_session(parent, session, lawsuit); break;
		case CM_CLERK_EDIT_SET : result = edit_set_session(parent, session, lawsuit); break;
		case CM_CLERK_RESET : result = reset_session(parent, session, lawsuit); break;
		case CM_CLERK_UNSET : result = unset_session(parent, session, lawsuit); break;
	#undef session

	#define decision TYPECHECK_PTR_CAST(TDecision, container)
		case CM_CLERK_EDIT_JUDICIAL_ACT : result = edit_act(parent, decision, lawsuit); break;
		case CM_SECR_FORCE_DATE : result = force_decision(parent, decision, lawsuit); break;
	#undef decision

#undef lawsuit

	#define surround TYPECHECK_PTR_CAST(TSurround, container)
		case CM_REG_EDIT_SURROUND : result = edit_surround(parent, surround, PrimaryGroup(TRCDKeyContainer)); break;
		case CM_ADMIN_RENUMBER_SURROUND : result = renumber_surround(parent, surround); break;
	#undef surround

	#define inReg TYPECHECK_PTR_CAST(TInReg, container)
		case CM_REG_IN_EDIT : result = edit_inreg(parent, inReg); break;
		case CM_ADMIN_RENUMBER_INREG : result = renumber_inreg(parent, inReg); break;
	#undef inReg

	#define inRegEvent TYPECHECK_PTR_CAST(TInRegEvent, container)
		case CM_REG_EDIT_INREG_EVENT : result = edit_inreg_event(parent, inRegEvent); break;
	#undef inRegEvent

	#define outReg TYPECHECK_PTR_CAST(TOutReg, container)
		case CM_REG_OUT_EDIT : result = edit_outreg(parent, outReg); break;
		case CM_REG_OUT_RECEIVE : result = receive_out(parent, outReg); break;
	#undef outReg

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL

	#define prove TYPECHECK_PTR_CAST(TProve, container)
		case CM_ARCH_EDIT_PROVE : result = edit_prove(parent, prove); break;
	#undef prove

	#define proveAction TYPECHECK_PTR_CAST(TProveAction, container)
		case CM_ARCH_EDIT_PROVE_ACTION : result = edit_prove_action(parent, proveAction); break;
	#undef proveAction

#endif  // APPEAL || AREAL || MILITARY || REGIONAL

		default : error("%d: command not supported by electric action", command);
	}

	if (!result)
		*container << backup;
}

void electra_action(TWindow *parent, int command, TRCDKeyContainer *container)
{
	switch (command)
	{
	#define request TYPECHECK_PTR_CAST(TRequest, container)
		case CM_CLERK_NEW_REQUEST_RESOLUTION : new_request_resolution(parent, request); break;
		case CM_CLERK_NEW_LAWSUIT : new_lawsuit(parent, request); break;
	#undef request

	#define lawsuit TYPECHECK_PTR_CAST(TLawsuit, container)
		case CM_CLERK_SET :
		{
			TSession session;
			set_session(parent, &session, lawsuit, NULL, NULL);
			break;
		}
		case CM_CLERK_CLOSED_RESULT : closed_result(parent, lawsuit, NULL); break;
		case CM_ARCH_NEW_ARCHIVE : new_archive(parent, lawsuit); break;
		case CM_ADMIN_DELETE_LAWSUIT_MOVEMENT : delete_movement(parent, lawsuit); break;
	#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		case CM_ADMIN_DELETE_EISPP_NMR : delete_eispp_nmr(parent, lawsuit); break;
	#endif  // APPEAL || AREAL || MILITARY || REGIONAL
		case CM_ADMIN_DELETE_LAWSUIT : delete_lawsuit(parent, lawsuit); break;
	#undef lawsuit

	#define resolution TYPECHECK_PTR_CAST(TRequestResolution, container)
		case CM_ADMIN_DELETE_REQUEST_RESOLUTION : delete_resolution(parent, resolution); break;
	#undef resolution

#define lawsuit PrimaryGroup(TLawsuit)

	#define session TYPECHECK_PTR_CAST(TSession, container)
		case CM_CLERK_PRINT_MESSAGES : edit_sides(parent, session, lawsuit); break;
		case CM_CLERK_NEW_JUDICIAL_ACT :
		{
			if (decompressKind(session))
			{
				DtorCompress cps(session);
				enter_act(parent, session, lawsuit);
			}
			else
				enter_act(parent, session, lawsuit);
			break;
		}
	#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
		case CM_REG_IN_SURROUENDOCS :
		{
			decompressKind(session);
			DtorCompress cps(session);

			if (!appeal_surrouendocs(parent, session, lawsuit))
				error("Няма вписани актове по възраженията.");
			break;
		}
	#endif  // APPEAL || AREAL
	#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		case CM_CLERK_NEW_DISTRIB_JURORS : new_distrib_jurors(parent, session, lawsuit); break;
		case CM_CLERK_EDIT_DISTRIB_JURORS : edit_distrib_jurors(parent, session, lawsuit); break;
	#endif  // AREAL || MILITARY || REGIONAL

	#undef session

	#define decision TYPECHECK_PTR_CAST(TDecision, container)
		case CM_SECR_PRINT_MESSAGES : edit_sides(parent, decision, lawsuit, SIDE_MODE_EDIT); break;
	#undef decision

	#define container TYPECHECK_PTR_CAST(TIndexContainer, container)
		case CM_ADMIN_DELETE_LAWSUIT_EVENT : delete_lawsuit_event(parent, lawsuit, container); break;
	#undef container

#undef lawsuit

	#define inReg TYPECHECK_PTR_CAST(TInReg, container)
		case CM_REG_NEW_INREG_EVENT : new_inreg_event(parent, inReg); break;
	#undef inReg

	#define inRegEvent TYPECHECK_PTR_CAST(TInRegEvent, container)
		case CM_ADMIN_DELETE_INREG_EVENT : delete_inreg_event(parent, inRegEvent); break;
	#undef inRegEvent

	#define outReg TYPECHECK_PTR_CAST(TOutReg, container)
		case CM_ADMIN_DELETE_OUTREG_RETURN : delete_outreg_return(parent, outReg); break;
		case CM_ADMIN_DELETE_OUTREG : delete_outreg(parent, outReg); break;
	#undef outReg

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL

	#define prove TYPECHECK_PTR_CAST(TProve, container)
		case CM_ARCH_NEW_PROVE_ACTION : new_prove_action(parent, prove); break;
		case CM_ADMIN_DELETE_PROVE : delete_prove(parent, prove); break;
	#undef prove
		case CM_ARCH_NEW_PROVE : new_prove(parent, container); break;

	#define proveAction TYPECHECK_PTR_CAST(TProveAction, container)
		case CM_ADMIN_DELETE_PROVE_ACTION : delete_prove_action(parent, proveAction); break;
	#undef proveAction

#endif  // APPEAL || AREAL || MILITARY || REGIONAL

		case CM_REG_NEW_SURROUND : new_surround(parent, container); break;
		case CM_CLERK_EDIT_SCHEDS : edit_scheds(parent, container); break;
		case CM_CLERK_IN_OUT_MOVEMENT : movement(parent, container); break;
		case CM_ADMIN_DELETE_INDOC : delete_indoc(parent, container); break;
		default : electra_back_action(parent, command, container);
	}
}
