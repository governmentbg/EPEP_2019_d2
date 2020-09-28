#include "cac.h"

static bool find_inreg_side(const TRCDKey &key, TSideWind &sideWind)
{
	msql m(sideWind.Name);

	m.AddLong("F_NO", key.no);
	m.AddLong("F_YEAR", key.year);
	m.AddChar("F_TYPE", key.type);

	TQuery q(str(m));
	if (q.Read())
	{
		sideWind << q;
		return true;
	}

	return say_no_data(sideWind.Name);
}

static bool already_used(const char *email)
{
	msql m("T_INREG I JOIN T_INREG_EVENT IE ON IE.F_NO = I.F_NO AND IE.F_YEAR = I.F_YEAR AND IE.F_TYPE = I.F_TYPE", "F_NO");

	m.AddChar("I.F_KIND", KIND_EPORTAL_REQUEST);
	m.AddLong("I.F_EPORTAL_MODE", EPORTAL_REGISTER);
	m.AddChar("IE.F_RESULT", RESULT_EPORTAL_ALLOWED);
	m.AddString("I.F_EPORTAL_MAIL", email);
	TQuery q(str(m));
	return q.Read();
}

static sign check_registration(const TUCNGroup &ucnGroup, char involvement)
{
	if (eportal_guid(&ucnGroup, involvement))
		return -1;

	msql m("T_SIDE_WIND W JOIN T_INREG I ON I.F_NO = W.F_NO AND I.F_YEAR = W.F_YEAR AND I.F_TYPE = W.F_TYPE "
		"JOIN T_INREG_EVENT IE ON IE.F_NO = W.F_NO AND IE.F_YEAR = W.F_YEAR AND IE.F_TYPE = W.F_TYPE", "F_NO");

	m.AddString("W.F_UCN", ucnGroup.ucn);
	m.AddChar("W.F_UCN_TYPE", ucnGroup.ucnType);
	m.AddChar("W.F_TYPE", TYPE_INREG);
	m.AddChars(strchr(INVOLVEMENT_LAWERS, involvement) ? "W.F_INVOLVEMENT" : "NOT W.F_INVOLVEMENT", INVOLVEMENT_LAWERS);
	m.AddChar("I.F_KIND", KIND_EPORTAL_REQUEST);
	m.AddLong("I.F_EPORTAL_MODE", EPORTAL_REGISTER);
	m.AddChar("IE.F_RESULT", RESULT_EPORTAL_ALLOWED);

	TQuery q(str(m));
	return q.Read();
}

#include "InRegEventSideListDialog.h"
#include "InRegEventSideListDialog.cpp"

static void finish_inreg_event(TWindow *parent, TInReg *inReg, TInRegEvent *inRegEvent, long sideMode, TSideWind &sideWind)
{
	if (inReg->kind == KIND_EPORTAL_REQUEST)
	{
		if (inReg->eportalMode == EPORTAL_REGISTER)
		{
			// 2016:292 what -> involvement
			eportal_send(&sideWind, sideWind.involvement);	// always LOG_WRITE
		}
		else if (sideMode == SIDE_MODE_ADD)		// can't be EDIT (tm)
		{
			long what = involvement2assignment(sideWind.involvement);
			char oper = (inReg->eportalMode == EPORTAL_REVOKE) ? LOG_DELETE : LOG_WRITE;
			TEPortAssign assign;
			TGArray<TEPortAssign> assigns;

			assign.key = inReg->key;
			assign.LoadFloat(assigns);

			for (int i = 0; i < assigns.Count(); i++)
				eportal_send(assigns[i], &sideWind, oper, what);
		}
	}
	else
		TInRegEventSideListDialog(parent, inRegEvent, inReg, sideMode).Execute();
}

#include "InRegEditDialog.h"
#include "InRegEditDialog.cpp"

#include "InRegSideListDialog.h"
#include "InRegSideListDialog.cpp"

#include "InRegEPortalDialog.h"
#include "InRegEPortalDialog.cpp"

#define assign OFFICE_GROUP_CAST(TEPortAssign)

static void assign_law_type(mstr &m, TGroup *group) { m.cat(Type->Seek(assign->lawsuit.type)); }
static void assign_law_no_year(mstr &m, TGroup *group) { m.printf("%ld/%ld", assign->lawsuit.no, assign->lawsuit.year); }
static void assign_law_involvement(mstr &m, TGroup *group) { m.cat(Involvement->Seek(assign->involvement)); }

static void assign_eportal_subpoena(mstr &m, TGroup *group)
{
	m.cat(assign->flags & EPORTAL_SUBPOENA ? "Включено" : "Изключено");
}

#undef assign

#include "EPortalAssignListWindow.h"
#include "EPortalAssignListWindow.cpp"

#include "EPortalAssignListDialog.h"
#include "EPortalAssignListDialog.cpp"

bool edit_inreg(TWindow *parent, TInReg *inReg)
{
	if (inReg->kind == KIND_EPORTAL_REQUEST)
	{
		TSideWind sideWind;

		if (find_inreg_side(inReg->key, sideWind))
		{
			bool hasRegistration = check_registration(sideWind, sideWind.involvement);

			if (TInRegEPortalDialog(parent, inReg, true, hasRegistration, true, sideWind).Execute() == IDOK)
			{
				log(LOG_EDIT, CM_REG_IN_EDIT, inReg);

				if (inReg->eportalMode)
					TEPortalAssignListDialog(parent, inReg, &sideWind, true).Execute();

				return true;
			}
		}
	}
	else if (TInRegEditDialog(parent, inReg).Execute() == IDOK)
	{
		log(LOG_EDIT, CM_REG_IN_EDIT, inReg);
		TInRegSideListDialog(parent, inReg, NULL, SIDE_MODE_EDIT).Execute();
		return true;
	}

	return false;
}

#include "EPorUserAliasFace.h"
#include "EPorUserAliasFace.cpp"

#include "EditInRegEventDialog.h"
#include "EditInRegEventDialog.cpp"

bool edit_inreg_event(TWindow *parent, TInRegEvent *inRegEvent)
{
	TInReg *inReg = PrimaryGroup(TInReg);
	TSideWind sideWind;

	if (inReg->kind == KIND_EPORTAL_REQUEST && !find_inreg_side(inReg->key, sideWind))
		return false;

	if (TEditInRegEventDialog(parent, inReg, inRegEvent, true, sideWind).Execute() == IDOK)
	{
		log(LOG_EDIT, CM_REG_EDIT_INREG_EVENT, inRegEvent);
		finish_inreg_event(parent, inReg, inRegEvent, true, sideWind);
		return true;
	}

	return false;
}

#include "InRegNewBaseDialog.h"
#include "InRegNewBaseDialog.cpp"

#include "InRegNewDialog.h"
#include "InRegNewDialog.cpp"

void new_inreg(TWindow *parent, TInReg *inReg, const char *kinds, TLawsuit *lawsuit)
{
	if (!lawsuit->key.type)
		lawsuit->key.year = 0;

	if (!kinds)
		kinds = lawsuit->key.type ? KIND_INREG_NON_EPORTS : KIND_TEXTUAL_NON_EPORTS;

	inReg->lawsuit = lawsuit->key;

	if (TInRegNewDialog(parent, inReg, kinds, lawsuit).Execute() == IDOK)
	{
		log(LOG_NEW_DOC, CM_REG_IN_NEW, inReg);

		if (strchr(KIND_APPEAL_IN_DOCS, inReg->kind))
			xfer_email(inReg);	// 2011:320 FIX: mail law-related only

		TInRegSideListDialog(parent, inReg, lawsuit, SIDE_MODE_ADD).Execute();
	}
}

void new_inreg_event(TWindow *parent, TInReg *inReg)
{
	TInRegEvent inRegEvent;
	TSideWind sideWind;

	if (inReg->kind == KIND_EPORTAL_REQUEST && !find_inreg_side(inReg->key, sideWind))
		return;

	// 2005:318 LPR: replaced << inreg and text clear w/ key assignment
	inRegEvent.key = inReg->key;

	if (TEditInRegEventDialog(parent, inReg, &inRegEvent, false, sideWind).Execute() == IDOK)
	{
		log_event(LOG_ENTER, CM_REG_NEW_INREG_EVENT, &inRegEvent);

		if (inRegEvent.Status() == RESULT_INRETINV || inRegEvent.Status() == RESULT_INSEND)
			unschedule(SCHED_INHOLD, inReg);

		finish_inreg_event(parent, inReg, &inRegEvent, false, sideWind);
	}
}

#include "InRegNewXDialog.h"
#include "InRegNewXDialog.cpp"

void new_inreg_x(TWindow *parent, TInReg *inReg)
{
	if (TInRegNewXDialog(parent, inReg, KIND_IN_REG_X_DOCS).Execute() == IDOK)
	{
		log(LOG_NEW_DOC, CM_REG_IN_NEW_X, inReg, "преписки и разпореждания");
		xfer_email(inReg);
		TInRegSideListDialog(parent, inReg, NULL, SIDE_MODE_ADD).Execute();
	}
}

static bool check_personal_no(TWindow *parent, const TTransGroup &ucnGroup)
{
	TKnownPerson person;

	person << ucnGroup;
	person.involvement = INVOLVEMENT_LEGALESE;
	return person.Get() && edit_lawer_data(parent, &person, true);
}

static bool check_sides(const TTransGroup &ucnGroup, char involvement)
{
	msql m("T_SIDE_WIND W JOIN T_INVOLVEMENT I ON I.F_INVOLVEMENT = W.F_INVOLVEMENT", "W.F_NO");

	m.AddString("W.F_UCN", ucnGroup.ucn);
	m.AddChar("W.F_UCN_TYPE", ucnGroup.ucnType);
	m.AddChars("(W.F_KIND", KIND_LAWSUITS);
	m.NextOr();
	m.AddChars("W.F_KIND", KIND_SESSIONS_ENDOCS);
	m.cat(")");
	m.AddChars(strchr(INVOLVEMENT_LAWERS, involvement) ? "W.F_INVOLVEMENT" : "NOT W.F_INVOLVEMENT", INVOLVEMENT_LAWERS);
	m.AddIsNull("I.F_XFER", false);

	TQuery q(str(m));
	return q.Read();
}

void new_inreg_eportal(TWindow *parent, TInReg *inReg)
{
	TTransGroup ucnGroup;

	if (get_ucn(parent, &ucnGroup, UCN_CITIZENS, INVOLVEMENT_EPORTALS, true, false, -1))
	{
		bool hasSides = check_sides(ucnGroup, ucnGroup.involvement);
		sign hasRegistration = check_registration(ucnGroup, ucnGroup.involvement);

		if (!hasSides)
		{
			static constant not_proper_side = "не е страна по дело, заседание или съдебен акт в текущия съд, "
				"или участието му е неподходящо";

			if (hasRegistration)
			{
				error("Лицето има %s в %s, но %s. Регистрирайте го като страна с подходящо участие, и ще можете "
					"да му дадете достъп до дело.",
					hasRegistration == -1 ? "регистрация" : "одобрена заявка за регистрация",
					EPORTAL_NAME, not_proper_side);
				return;
			}
			else if (!ask("Лицето %s. %s", not_proper_side, Q_CONTINUE))
				return;
		}
		else if (hasRegistration)
			inReg->eportalMode = EPORTAL_GRANT;

		TSideWind sideWind;
		sideWind << ucnGroup;

		if ((!strchr(INVOLVEMENT_LAWERS, ucnGroup.involvement) || check_personal_no(parent, ucnGroup)) &&
			TInRegEPortalDialog(parent, inReg, false, hasRegistration, hasSides, sideWind).Execute() == IDOK)
		{
			log(LOG_NEW_DOC, CM_REG_IN_NEW_EPORTAL, inReg);
			sideWind << inReg;
			sideWind.Insert();

			if (inReg->eportalMode)
				TEPortalAssignListDialog(parent, inReg, &sideWind, false).Execute();
			else
				message("%s %ld/%ldг.", Type->Seek(inReg->key.type), inReg->key.no, inReg->key.year);
		}
	}
}

#include "InRegNewSurrouendocDialog.h"
#include "InRegNewSurrouendocDialog.cpp"

void new_inreg(TWindow *parent, const TLawsuit *lawsuit, const TSession *session, const TSurroundment *surrouendoc)
{
	TInReg inReg;

	inReg.lawsuit = lawsuit->key;
	inReg.decision.year = lawsuit->key.year;
	inReg.decision.type = lawsuit->key.type;
	inReg.decisionDate = session->date;
	inReg.decisionKind = surrouendoc->decisionKind;
	inReg.sessionKind = session->kind;
	inReg.surroundKind = surrouendoc->kind;
	inReg.surroundNo = surrouendoc->key.no;
	inReg.surroundYear = surrouendoc->key.year;
	inReg.extraNo = surrouendoc->extraNo;

	if (TInRegNewSurrouendocDialog(parent, &inReg, KIND_IN_SURROUNDERS, lawsuit, session, surrouendoc).Execute() == IDOK)
	{
		log(LOG_NEW_DOC, CM_REG_IN_NEW_X, &inReg, "актове по възражения");
		xfer_email(&inReg);
		TInRegSideListDialog(parent, &inReg, NULL, SIDE_MODE_ADD).Execute();
		// asserts
		cac_assert(inReg.lawsuit == lawsuit->key);
		cac_assert(inReg.decision.year == lawsuit->key.year);
		cac_assert(inReg.decision.type == lawsuit->key.type);
		cac_assert(inReg.decisionDate == session->date);
		cac_assert(inReg.decisionKind == surrouendoc->decisionKind);
		cac_assert(inReg.sessionKind == session->kind);
		cac_assert(inReg.surroundKind == surrouendoc->kind);
		cac_assert(inReg.surroundNo == surrouendoc->key.no);
		cac_assert(inReg.surroundYear == surrouendoc->key.year);
		cac_assert(inReg.extraNo == surrouendoc->extraNo);
		cac_assert(inReg.receiverType == UCN_INSTANCE_HIGHER);
		cac_assert(atob(inReg.receiver));
	}
}
